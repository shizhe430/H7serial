#include <esp_rmaker_work_queue.h>
/*
 * Smart Water Dispenser - ESP32-C6 with ESP RainMaker
 *
 * Features:
 *  - ESP RainMaker BLE provisioning (works with RainMaker phone app)
 *  - RainMaker cloud control (Power on/off, volume setting)
 *  - STM32 UART communication for hardware control
 *  - Optional web dashboard for local control
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_sntp.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "hal/gpio_hal.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_mac.h"
#include "esp_system.h"

/* ESP RainMaker headers */
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_ota.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_scenes.h>
#include <esp_rmaker_console.h>

/* App network for BLE provisioning */
#include <app_network.h>

/* DeepSeek AI assistant */
#include "deepseek.h"
#include "cJSON.h"

/* Console for serial commands */
#include "esp_console.h"

/* Time for day tracking */
#include <time.h>

/* DeepSeek globals (used by api_ai_handler + deepseek_worker_task) */
#define DEEPSEEK_QUESTION_SIZE 512
static SemaphoreHandle_t deepseek_sem = NULL;
static SemaphoreHandle_t deepseek_done_sem = NULL;
static char deepseek_question[DEEPSEEK_QUESTION_SIZE];
static char deepseek_reply[DEEPSEEK_RESPONSE_MAX_LEN];

static const char *TAG = "Dispenser";

/* ==================== STM32 UART Config ==================== */
/* UART1 on GPIO4(TX)/GPIO5(RX) - connect STM32 to these pins */
#define STM32_UART_PORT   UART_NUM_1
#define STM32_TX_PIN      GPIO_NUM_4
#define STM32_RX_PIN      GPIO_NUM_5
#define RX_BUF_SIZE       1024

/* ==================== Water Dispenser State ==================== */
static bool water_valve_open = false;
static int water_volume = 0;        /* cumulative ml dispensed */
static int last_dispensed_volume = 0; /* last dispense amount (ml) */
static int water_set_volume = 200;  /* ml per cycle */

/* Daily water tracking */
static int daily_water_target = 2000; /* default — AI will personalize */

/* ==================== User Management (Fingerprint / Voice ID) ==================== */
#define MAX_USERS                   50
#define FP_UNKNOWN                  -1   /* no user selected / guest */
/* UART commands from STM32 */
#define STM_FP_PREFIX               "STM_FP:"
#define STM_FP_NEW_PREFIX           "STM_FP_NEW:"
#define STM_VOICE_PREFIX            "STM_VOICE:"

typedef struct {
    uint8_t id;              /* fingerprint ID (1-255), 0 = slot empty */
    char    name[32];         /* display name (UTF-8) */
    int     gender;           /* 0=未设, 1=男, 2=女 */
    int     age;
    float   height_cm;        /* 身高(cm) */
    float   weight_kg;        /* 体重(kg) */
    int     daily_water;      /* ml consumed today */
    int     use_count;        /* number of dispenses today */
    int     total_water;      /* lifetime total (ml) */
} user_record_t;

static user_record_t users[MAX_USERS];
static int  user_count = 0;
static int  current_user_id = FP_UNKNOWN;
static int  saved_day_stamp = 0;       /* last day-of-year we saved to NVS */
static int  saved_web_date = 0;        /* last date (yyyymmdd) we saw from web (authoritative for day reset) */

/* NVS namespace for user data */
#define USERS_NVS   "water_users"

/* Forward declarations for functions used before definition */
static user_record_t* find_user_by_id(int fp_id);
static user_record_t* find_user_by_name(const char *name);
static user_record_t* add_user(int fp_id, const char *name);
static void save_users_to_nvs(void);

/* Forward declarations for missing functions */
static void save_users_full(void);
static void sync_user_info_to_web(int fp_id);
static void fetch_user_info_from_web(int fp_id);
static void align_with_web(int fp_id);
static void load_users_from_nvs(void);
static void init_default_users(void);
static void check_day_change(void);
static void start_web_server(void);
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                           const esp_rmaker_param_val_t val, void *priv_data,
                           esp_rmaker_write_ctx_t *ctx);
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data);
static void register_ask_command(void);
static void voice_broadcast(const char *text);
static void notify_daily_water(int dispensed_ml);
static int calculate_daily_target(user_record_t *u);
static void update_all_params(void);
static void send_advice(void);
static void send_user_info(void);
static void advice_worker_task(void *arg);
static void generate_advice_and_send(void);
static SemaphoreHandle_t advice_sem = NULL;
static SemaphoreHandle_t suggest_sem = NULL;
static char suggest_question[512];
static bool auto_suggest_enabled = true;

/* RainMaker device handles */
static esp_rmaker_device_t *dispenser_device = NULL;
static esp_rmaker_param_t *power_param = NULL;
static esp_rmaker_param_t *daily_param = NULL;
static esp_rmaker_param_t *last_vol_param = NULL;
static esp_rmaker_param_t *count_param = NULL;
static esp_rmaker_param_t *user_id_param = NULL;
static esp_rmaker_param_t *remain_param = NULL;
static esp_rmaker_param_t *userinfo_param = NULL;  /* 用户信息录入（可写，App填姓名/年龄/身高/体重） */

/* HTTP server */
static httpd_handle_t server = NULL;
static char device_ip[16] = "0.0.0.0";

/* ==================== 本地服务器配置（双发模式） ==================== */
/* 你的饮水机网页服务器地址，换网络后改这里 */
#define LOCAL_SERVER_URL CONFIG_WATER_WEB_SERVER_URL
/* ESP32用户ID → 网页服务器user_id 映射表 */
/* 格式: {esp32_fp_id, web_user_id}，末尾用{0,0}结束 */
/* 乐鑫App用户ID → 网页user_id 一一对应 */
/* 贺大爷=1, 李奶奶=2, 王叔叔=3 */
static const int fp_to_web_id[][2] = {
    {1, 1},
    {2, 2},
    {3, 3},
    {4, 202},
    {0, 0}  /* 结束标记 */
};

/* Dispense worker queue */
typedef struct {
    int vol;
    int fp_id;
    int set_fp;
} dispense_event_t;
static QueueHandle_t dispense_queue = NULL;


/* ==================== User Management Functions ==================== */
static user_record_t* find_user_by_id(int fp_id)
{
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].id == fp_id) return &users[i];
    }
    return NULL;
}

static user_record_t* find_user_by_name(const char *name)
{
    if (!name || !*name) return NULL;
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].id != 0 && strcasecmp(users[i].name, name) == 0)
            return &users[i];
    }
    return NULL;
}

static int find_free_slot(void)
{
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].id == 0) return i;
    }
    return -1;
}

static user_record_t* add_user(int fp_id, const char *name)
{
    if (user_count >= MAX_USERS) return NULL;
    int slot = find_free_slot();
    if (slot < 0) return NULL;
    users[slot].id = fp_id;
    strncpy(users[slot].name, name ? name : "", sizeof(users[slot].name) - 1);
    users[slot].daily_water = 0;
    users[slot].use_count = 0;
    users[slot].total_water = 0;
    user_count++;
    save_users_to_nvs();
    return &users[slot];
}

static void save_users_to_nvs(void)
{
    nvs_handle_t h;
    if (nvs_open(USERS_NVS, NVS_READWRITE, &h) != ESP_OK) return;
    nvs_set_blob(h, "cnt", &user_count, sizeof(user_count));
    nvs_set_blob(h, "day", &saved_day_stamp, sizeof(saved_day_stamp));
    nvs_set_blob(h, "wdate", &saved_web_date, sizeof(saved_web_date));
    nvs_commit(h);
    nvs_close(h);
}

static void save_users_full(void)
{
    nvs_handle_t h;
    if (nvs_open(USERS_NVS, NVS_READWRITE, &h) != ESP_OK) return;
    nvs_set_blob(h, "users", users, sizeof(users));
    nvs_commit(h);
    nvs_close(h);
}

static void load_users_from_nvs(void)
{
    nvs_handle_t h;
    if (nvs_open(USERS_NVS, NVS_READONLY, &h) != ESP_OK) return;
    size_t sz = sizeof(user_count);
    if (nvs_get_blob(h, "cnt", &user_count, &sz) != ESP_OK) user_count = 0;
    sz = sizeof(users);
    if (nvs_get_blob(h, "users", users, &sz) != ESP_OK) memset(users, 0, sizeof(users));
    sz = sizeof(saved_day_stamp);
    if (nvs_get_blob(h, "day", &saved_day_stamp, &sz) != ESP_OK) saved_day_stamp = 0;
    sz = sizeof(saved_web_date);
    if (nvs_get_blob(h, "wdate", &saved_web_date, &sz) != ESP_OK) saved_web_date = 0;
    nvs_close(h);
}

static void init_default_users(void)
{
    if (find_user_by_id(1)) return;
    static const struct {
        int id; const char *name; int gender; int age; float h; float w;
    } preset[] = {
        {1, "\xe7\x94\xa8\xe6\x88\xb7" "1", 1, 20, 178, 67},
        {2, "\xe7\x94\xa8\xe6\x88\xb7" "2", 1, 20, 181, 78},
        {3, "\xe7\x94\xa8\xe6\x88\xb7" "3", 1, 20, 174, 60},
    };
    for (int i = 0; i < 3; i++) {
        user_record_t *u = add_user(preset[i].id, preset[i].name);
        if (u) {
            u->gender = preset[i].gender;
            u->age = preset[i].age;
            u->height_cm = preset[i].h;
            u->weight_kg = preset[i].w;
        }
    }
    save_users_full();
}

static void check_day_change(void)
{
    time_t now;
    time(&now);
    struct tm *t = localtime(&now);
    if (t->tm_year < 100) return;
    int today = t->tm_yday;
    if (saved_day_stamp == 0) {
        saved_day_stamp = today;
        save_users_to_nvs();
        return;
    }
    if (today != saved_day_stamp) {
        for (int i = 0; i < MAX_USERS; i++) {
            users[i].daily_water = 0;
            users[i].use_count = 0;
        }
        update_all_params();
        save_users_full();
    }
    saved_day_stamp = today;
}

static int calculate_daily_target(user_record_t *u)
{
    if (!u || u->weight_kg < 20) return 1700;
    int base = (u->gender == 2) ? 1500 : 1700;
    int water = (int)(base * u->weight_kg / 65.0f);
    if (water < 700) water = 700;
    if (water > 3000) water = 3000;
    return water;
}

static void _rmaker_update(void *priv)
{
    user_record_t *u = find_user_by_id(current_user_id);
    int uid = (current_user_id == FP_UNKNOWN) ? 0 : current_user_id;
    if (user_id_param) esp_rmaker_param_update_and_report(user_id_param, esp_rmaker_int(uid));
    if (last_vol_param) esp_rmaker_param_update_and_report(last_vol_param, esp_rmaker_int(last_dispensed_volume));
    if (count_param) esp_rmaker_param_update_and_report(count_param, esp_rmaker_int(u ? u->use_count : 0));
    int daily = u ? u->daily_water : 0;
    if (daily_param) esp_rmaker_param_update_and_report(daily_param, esp_rmaker_int(daily));
    int target = calculate_daily_target(u);
    int remain = target - daily;
    if (remain < 0) remain = 0;
    if (remain_param) esp_rmaker_param_update_and_report(remain_param, esp_rmaker_int(remain));
}

static void update_all_params(void)
{
    _rmaker_update(NULL);
}

/* ==================== RainMaker Write Callback ==================== */
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                           const esp_rmaker_param_val_t val, void *priv_data,
                           esp_rmaker_write_ctx_t *ctx)
{
    const char *pname = esp_rmaker_param_get_name(param);
    if (strcmp(pname, "\xe5\xbc\x80\xe5\x85\xb3") == 0) {
        ESP_LOGI(TAG, "App switch -> %s", val.val.b ? "ON" : "OFF");
        if (val.val.b) {
            water_valve_open = true;
            uart_write_bytes(STM32_UART_PORT, "APP_CMD:START\r\n", 15);
        } else {
            water_valve_open = false;
            uart_write_bytes(STM32_UART_PORT, "APP_CMD:STOP\r\n", 14);
        }
        esp_rmaker_param_update_and_report(param, val);
    } else if (strcmp(pname, "\xe7\x94\xa8\xe6\x88\xb7\xe4\xbf\xa1\xe6\x81\xaf\xe5\xbd\x95\xe5\x85\xa5(\xe5\xa7\x93\xe5\x90\x8d,\xe5\xb9\xb4\xe9\xbe\x84,\xe8\xba\xab\xe9\xab\x98,\xe4\xbd\x93\xe9\x87\x8d)") == 0) {
        /* 用户信息录入：App 填 "姓名,年龄,身高,体重" → 存当前用户 NVS → 同步 Web */
        const char *s = val.val.s ? val.val.s : "";
        char buf[96];
        strncpy(buf, s, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        char *p1 = strchr(buf, ',');
        char *p2 = p1 ? strchr(p1 + 1, ',') : NULL;
        char *p3 = p2 ? strchr(p2 + 1, ',') : NULL;
        user_record_t *u = find_user_by_id(current_user_id);
        if (u && p3) {
            *p1 = '\0'; *p2 = '\0'; *p3 = '\0';
            char *name = buf, *age = p1 + 1, *height = p2 + 1, *weight = p3 + 1;
            if (name[0]) strncpy(u->name, name, sizeof(u->name) - 1);
            if (age[0]) u->age = atoi(age);
            if (height[0]) u->height_cm = atof(height);
            if (weight[0]) u->weight_kg = atof(weight);
            save_users_full();
            ESP_LOGI(TAG, "\xe7\x94\xa8\xe6\x88\xb7\xe4\xbf\xa1\xe6\x81\xaf\xe5\xbd\x95\xe5\x85\xa5: %s,%s,%s,%s", name, age, height, weight);
            /* 同步到 Web 数据库 */
            sync_user_info_to_web(current_user_id);
        } else {
            ESP_LOGW(TAG, "\xe6\x97\xa0\xe5\xbd\x93\xe5\x89\x8d\xe7\x94\xa8\xe6\x88\xb7\xef\xbc\x8c\xe4\xbf\xa1\xe6\x81\xaf\xe5\xbd\x95\xe5\x85\xa5\xe5\xa4\xb1\xe8\xb4\xa5");
        }
        esp_rmaker_param_update_and_report(param, val);
    }
    return ESP_OK;
}

/* ==================== RainMaker Event Handler ==================== */
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == RMAKER_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_INIT_DONE: break;
            case RMAKER_EVENT_CLAIM_SUCCESSFUL: break;
            case RMAKER_EVENT_STARTED: break;
            default: break;
        }
    } else if (event_base == RMAKER_OTA_EVENT) {
        switch (event_id) {
            case RMAKER_OTA_EVENT_STARTING: break;
            case RMAKER_OTA_EVENT_SUCCESSFUL: break;
            case RMAKER_OTA_EVENT_REQ_FOR_REBOOT: break;
            default: break;
        }
    }
}

/* ==================== Web Server ==================== */
static const char *DASHBOARD_HTML =
"<!DOCTYPE html><html><head><meta charset='utf-8'>"
"<meta name='viewport' content='width=device-width,initial-scale=1'>"
"<title>\xe6\x99\xba\xe8\x83\xbd\xe9\xa5\xae\xe6\xb0\xb4\xe6\x9c\xba</title><style>"
"*{box-sizing:border-box;margin:0;padding:0}"
"body{font-family:-apple-system,BlinkMacSystemFont,sans-serif;background:#f0f2f5;padding:16px}"
".card{background:white;border-radius:16px;padding:24px;margin:12px 0;box-shadow:0 2px 8px rgba(0,0,0,0.1)}"
"h1{font-size:24px;color:#333;text-align:center;margin-bottom:8px}"
".status{padding:12px;border-radius:10px;text-align:center;font-size:18px;font-weight:bold}"
".on{background:#e8f5e9;color:#2e7d32}.off{background:#fbe9e7;color:#c62828}"
".btn{display:block;width:100%;padding:18px;font-size:20px;border:none;border-radius:12px;cursor:pointer;font-weight:bold;margin:8px 0}"
".btn-primary{background:#1976d2;color:white}.btn-danger{background:#d32f2f;color:white}"
".label{font-size:14px;color:#666;margin-top:12px}"
".info{background:#e3f2fd;border-radius:10px;padding:12px;text-align:center;color:#1565c0;font-size:14px;margin:8px 0}"
"</style></head><body>"
"<h1>\xe6\x99\xba\xe8\x83\xbd\xe9\xa5\xae\xe6\xb0\xb4\xe6\x9c\xba</h1>"
"<div class='card'>"
"<div id='status' class='status off'>\xe5\xb7\xb2\xe5\x81\x9c\xe6\xad\xa2</div>"
"<div class='label'>\xe4\xb8\x8a\xe6\xac\xa1\xe5\x87\xba\xe6\xb0\xb4\xe9\x87\x8f: <span id='lastVol' style='font-weight:bold'>--</span> ml</div>"
"<button class='btn btn-primary' onclick='onWater()'>\xe5\x8f\x96\xe6\xb0\xb4</button>"
"<button class='btn btn-danger' onclick='offWater()'>\xe5\x81\x9c\xe6\xad\xa2</button>"
"<div class='label'>\xe4\xbb\x8a\xe6\x97\xa5\xe4\xbd\xbf\xe7\x94\xa8\xe6\xac\xa1\xe6\x95\xb0: <span id='totalVol'>0</span> \xe6\xac\xa1</div>"
"</div>"
"<div class='card'>"
"<div class='info'>IP: <span id='deviceIP'>%s</span></div>"
"</div>"
"<script>"
"function refresh(){fetch('/api/status').then(r=>r.json()).then(d=>{"
"document.getElementById('status').className='status '+(d.valve?'on':'off');"
"document.getElementById('status').innerText=d.valve?'\xe5\x87\xba\xe6\xb0\xb4\xe4\xb8\xad':'\xe5\xb7\xb2\xe5\x81\x9c\xe6\xad\xa2';"
"document.getElementById('totalVol').innerText=d.useCount;"
"document.getElementById('lastVol').innerText=d.lastVol||'--';"
"})}"
"function onWater(){fetch('/api/on').then(refresh)}"
"function offWater(){fetch('/api/off').then(refresh)}"
"setInterval(refresh,3000);refresh()"
"</script></body></html>";

static esp_err_t api_status_handler(httpd_req_t *req) {
    user_record_t *su = find_user_by_id(current_user_id);
    int uc = su ? su->use_count : 0;
    char buf[128];
    snprintf(buf, sizeof(buf), "{\"valve\":%s,\"volume\":%d,\"lastVol\":%d,\"useCount\":%d}",
             water_valve_open ? "true" : "false", water_volume, last_dispensed_volume, uc);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, buf, strlen(buf));
    return ESP_OK;
}

static esp_err_t api_on_handler(httpd_req_t *req) {
    water_valve_open = true;
    uart_write_bytes(STM32_UART_PORT, "APP_CMD:START\r\n", 15);
    if (power_param) esp_rmaker_param_update_and_report(power_param, esp_rmaker_bool(true));
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{\"status\":\"on\"}", 14);
    return ESP_OK;
}

static esp_err_t api_off_handler(httpd_req_t *req) {
    water_valve_open = false;
    uart_write_bytes(STM32_UART_PORT, "APP_CMD:STOP\r\n", 14);
    if (power_param) esp_rmaker_param_update_and_report(power_param, esp_rmaker_bool(false));
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{\"status\":\"off\"}", 15);
    return ESP_OK;
}

static esp_err_t api_ai_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{\"reply\":\"AI \xe5\xb7\xa5\xe5\x85\xb7\xe6\x9a\x82\xe5\x81\x9c\"}", 23);
    return ESP_OK;
}

static esp_err_t root_handler(httpd_req_t *req) {
    char *html = malloc(strlen(DASHBOARD_HTML) + 32);
    if (!html) { httpd_resp_send_404(req); return ESP_FAIL; }
    snprintf(html, strlen(DASHBOARD_HTML) + 32, DASHBOARD_HTML, device_ip);
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, strlen(html));
    free(html);
    return ESP_OK;
}

static esp_err_t api_daily_handler(httpd_req_t *req);
static esp_err_t api_users_handler(httpd_req_t *req);
static esp_err_t api_set_user_name_handler(httpd_req_t *req);
static esp_err_t api_select_user_handler(httpd_req_t *req);
static esp_err_t api_sim_handler(httpd_req_t *req);
static esp_err_t api_ask_handler(httpd_req_t *req);

static void start_web_server(void) {
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    cfg.server_port = 80;
    cfg.max_uri_handlers = 14;
    cfg.lru_purge_enable = true;
    cfg.stack_size = 16384;
    if (httpd_start(&server, &cfg) == ESP_OK) {
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/", .method=HTTP_GET, .handler=root_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/status", .method=HTTP_GET, .handler=api_status_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/on", .method=HTTP_GET, .handler=api_on_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/off", .method=HTTP_GET, .handler=api_off_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/ai", .method=HTTP_POST, .handler=api_ai_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/daily", .method=HTTP_GET, .handler=api_daily_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/users", .method=HTTP_GET, .handler=api_users_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/users/set_name", .method=HTTP_POST, .handler=api_set_user_name_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/users/select", .method=HTTP_POST, .handler=api_select_user_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/sim", .method=HTTP_GET, .handler=api_sim_handler});
        httpd_register_uri_handler(server, &(httpd_uri_t){.uri="/api/ask", .method=HTTP_GET, .handler=api_ask_handler});
    }
}

static void suggest_worker_task(void *arg)
{
    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
    while (1) {
        if (xSemaphoreTake(suggest_sem, portMAX_DELAY) == pdTRUE) {
            reply[0] = '\0';
            esp_err_t err = deepseek_query(suggest_question, reply, sizeof(reply));
            if (err == ESP_OK && reply[0]) {
                printf("[SUGGEST] %s\n", reply);
            } else {
                printf("[SUGGEST] FAIL: %s\n", esp_err_to_name(err));
            }
        }
    }
    vTaskDelete(NULL);
}

/* ==================== 发送饮水记录到本地服务器（双发模式） ==================== */
static int fp_to_web_user_id(int fp_id)
{
    for (int i = 0; fp_to_web_id[i][0] != 0; i++) {
        if (fp_to_web_id[i][0] == fp_id) return fp_to_web_id[i][1];
    }
    return 0; /* 未知用户，用0 */
}

/* 简易 URL 编码：仅保留字母数字及 -_.~ ，空格→+ ，其余 %XX（application/x-www-form-urlencoded 标准） */
static void url_encode(const char *src, char *dst, size_t dst_size)
{
    if (!src || !dst || dst_size == 0) return;
    size_t pos = 0;
    for (const char *p = src; *p && pos < dst_size - 4; p++) {
        unsigned char c = (unsigned char)*p;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')
            || c == '-' || c == '_' || c == '.' || c == '~') {
            dst[pos++] = (char)c;
        } else if (c == ' ') {
            dst[pos++] = '+';
        } else {
            pos += snprintf(dst + pos, dst_size - pos, "%%%02X", c);
        }
    }
    dst[pos] = '\0';
}

/* 待同步脏标记：出水后上报失败的用户，由 sync_retry_task 重试 */
static bool pending_sync[MAX_USERS];

static void send_to_local_server(int volume_ml, int fp_id);

static void mark_pending_sync(int fp_id, bool val)
{
    user_record_t *u = find_user_by_id(fp_id);
    if (!u) return;
    int slot = (int)(u - users);
    pending_sync[slot] = val;
}

static void send_to_local_server(int volume_ml, int fp_id)
{
    int web_user_id = fp_to_web_user_id(fp_id);
    if (web_user_id <= 0) {
        ESP_LOGI(TAG, "本地服务器: 跳过未知用户 fp_id=%d", fp_id);
        return;
    }

    /* 只上报增量：volume_ml 是本次出水量（0=纯同步探活），target_ml 是 C6 口径目标值。
       统计由网页端权威累加（TotalMl+=volume_ml / UseCount+=1），不再上报总量覆盖 */
    user_record_t *u = find_user_by_id(fp_id);
    int target      = calculate_daily_target(u);

    char post_url[256];
    snprintf(post_url, sizeof(post_url), "%s/api/records", LOCAL_SERVER_URL);

    char post_data[192];
    snprintf(post_data, sizeof(post_data),
             "user_id=%d&volume_ml=%d&target_ml=%d",
             web_user_id, volume_ml, target);

    esp_http_client_config_t config = {
        .url = post_url,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 5000,
        .skip_cert_common_name_check = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "本地服务器: HTTP客户端初始化失败");
        return;
    }

    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status = esp_http_client_get_status_code(client);
        if (status == 200) {
            ESP_LOGI(TAG, "本地服务器上报成功: fp_id=%d→web_id=%d, %dml, HTTP %d",
                     fp_id, web_user_id, volume_ml, status);
            mark_pending_sync(fp_id, false);
        } else {
            ESP_LOGW(TAG, "本地服务器上报 HTTP %d", status);
            mark_pending_sync(fp_id, true);
        }
    } else {
        ESP_LOGW(TAG, "本地服务器上报失败: %s (服务器未开机或网络不通)", esp_err_to_name(err));
        mark_pending_sync(fp_id, true);
    }

    esp_http_client_cleanup(client);
}

/* 重试/对账任务：网页为权威，改为"拉对齐"。
   每30s重试脏标记用户：从网页拉回权威值覆盖本地（网络恢复后自愈）。
   每5分钟全量对账：全用户拉回（兜底自愈 + 网页日期0点重置）。 */
static void sync_retry_task(void *arg)
{
    int tick = 0;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(30000));
        tick++;
        for (int i = 0; i < MAX_USERS; i++) {
            if (users[i].id != 0 && pending_sync[i]) {
                fetch_user_info_from_web(users[i].id);
                update_all_params();
            }
        }
        if (tick % 10 == 0) {  /* 约每5分钟全量对账 */
            for (int i = 0; i < MAX_USERS; i++) {
                if (users[i].id != 0) {
                    fetch_user_info_from_web(users[i].id);
                    update_all_params();
                }
            }
        }
    }
    vTaskDelete(NULL);
}

/* ==================== 同步用户信息到 Web 数据库（App录入后调用） ==================== */
static void sync_user_info_to_web(int fp_id)
{
    user_record_t *u = find_user_by_id(fp_id);
    if (!u) return;
    int web_user_id = fp_to_web_user_id(fp_id);
    if (web_user_id <= 0) {
        ESP_LOGI(TAG, "同步用户信息: 跳过未知用户 fp_id=%d", fp_id);
        return;
    }

    char post_url[256];
    snprintf(post_url, sizeof(post_url), "%s/api/users/update", LOCAL_SERVER_URL);

    /* user_id, name, age, height_cm, weight_kg, gender（姓名做 URL 编码，中文不乱码） */
    char enc_name[96];
    url_encode(u->name, enc_name, sizeof(enc_name));
    char post_data[256];
    snprintf(post_data, sizeof(post_data),
             "user_id=%d&name=%s&age=%d&height_cm=%.1f&weight_kg=%.1f&gender=%d",
             web_user_id, enc_name, u->age, u->height_cm, u->weight_kg, u->gender);

    esp_http_client_config_t config = {
        .url = post_url,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 5000,
        .skip_cert_common_name_check = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "同步用户信息: HTTP客户端初始化失败");
        return;
    }

    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "同步用户信息成功: fp_id=%d → web_id=%d, HTTP %d",
                 fp_id, web_user_id, status);
    } else {
        ESP_LOGW(TAG, "同步用户信息失败: %s (服务器未开机或网络不通)", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

/* ==================== 从 Web 拉取今日饮水统计（识别用户后调用） ==================== */
/* 流程：拉回网页权威值（today_total_ml/today_use_count/target_ml/needed_ml/user_name）覆盖本地，
   并用网页的 today_date 做权威 0 点重置（网页每天新行，日期变更即清零所有用户当天统计） */
static void fetch_user_info_from_web(int fp_id)
{
    int web_user_id = fp_to_web_user_id(fp_id);
    if (web_user_id <= 0) return;

    char get_url[256];
    snprintf(get_url, sizeof(get_url), "%s/api/summary?user_id=%d", LOCAL_SERVER_URL, web_user_id);

    esp_http_client_config_t config = {
        .url = get_url,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 5000,
        .skip_cert_common_name_check = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "拉取今日饮水: HTTP客户端初始化失败");
        return;
    }

    /* GET 不能用 perform 后 read_response（perform 已消费响应体），改用 open/fetch/read 手动读 */
    esp_err_t err = esp_http_client_open(client, 0);
    if (err == ESP_OK) {
        int content_length = esp_http_client_fetch_headers(client);
        int status = esp_http_client_get_status_code(client);
        if (status == 200 && content_length >= 0) {
            /* 响应可能含较长的 advice/ai_advice，固定 2KB 缓冲循环读取（统计字段在最前，截断不影响解析） */
            char resp[2048];
            int total = 0;
            int rd;
            while (total < (int)sizeof(resp) - 1 &&
                   (rd = esp_http_client_read(client, resp + total, sizeof(resp) - total - 1)) > 0) {
                total += rd;
            }
            resp[total] = '\0';
            ESP_LOGI(TAG, "GET raw resp (%d bytes): %s", total, resp);
            /* 解析 JSON: today_date, today_total_ml, today_use_count, target_ml, needed_ml, user_name */
                    int today_total = 0, today_count = 0, target = 0, needed = 0;
                    int web_date = 0;
                    char uname[32] = {0};
                    char *p = strstr(resp, "\"today_date\":\"");
                    if (p) {
                        /* 格式 yyyy-MM-dd → 转成 int 20260807；用 strlen 算偏移，避免手数错位 */
                        const char *d = p + strlen("\"today_date\":\"");
                        int yy = (d[0]-'0')*1000 + (d[1]-'0')*100 + (d[2]-'0')*10 + (d[3]-'0');
                        int mm = (d[5]-'0')*10 + (d[6]-'0');
                        int dd = (d[8]-'0')*10 + (d[9]-'0');
                        if (yy >= 2000) web_date = yy * 10000 + mm * 100 + dd;
                    }
                    p = strstr(resp, "\"today_total_ml\":");
                    if (p) today_total = atoi(p + strlen("\"today_total_ml\":"));
                    p = strstr(resp, "\"today_use_count\":");
                    if (p) today_count = atoi(p + strlen("\"today_use_count\":"));
                    p = strstr(resp, "\"target_ml\":");
                    if (p) target = atoi(p + strlen("\"target_ml\":"));
                    p = strstr(resp, "\"needed_ml\":");
                    if (p) needed = atoi(p + strlen("\"needed_ml\":"));
                    p = strstr(resp, "\"user_name\":\"");
                    if (p) {
                        const char *start = p + strlen("\"user_name\":\"");
                        const char *end = strchr(start, '"');
                        if (end) {
                            size_t nl = end - start;
                            if (nl >= sizeof(uname)) nl = sizeof(uname) - 1;
                            memcpy(uname, start, nl);
                            uname[nl] = '\0';
                        }
                    }

                    /* 网页日期权威 0 点重置：网页 daily_summary 按天一行，日期变更即所有用户当天统计归零 */
                    if (web_date > 0 && saved_web_date != 0 && web_date != saved_web_date) {
                        ESP_LOGI(TAG, "网页日期变更 %d → %d，清零所有用户当天统计", saved_web_date, web_date);
                        for (int i = 0; i < MAX_USERS; i++) {
                            if (users[i].id != 0) {
                                users[i].daily_water = 0;
                                users[i].use_count = 0;
                            }
                        }
                        update_all_params();
                        save_users_full();
                    }
                    if (web_date > 0) saved_web_date = web_date;

                    user_record_t *u = find_user_by_id(fp_id);
                    if (u) {
                        /* 以网页权威值覆盖本地（网页是唯一累计源，拉回的就是正确累计值） */
                        u->daily_water = today_total;
                        u->use_count = today_count;
                        if (uname[0]) strncpy(u->name, uname, sizeof(u->name) - 1);
                        save_users_full();
                        ESP_LOGI(TAG, "从Web拉取今日饮水: ID=%d, %s, 日期%d, 今日%dml/%d次, 目标%dml, 还需%dml",
                                 fp_id, uname, web_date, today_total, today_count, target, needed);
                    }
        } else {
            ESP_LOGW(TAG, "拉取今日饮水: HTTP %d", status);
        }
    } else {
        ESP_LOGW(TAG, "拉取今日饮水失败: %s", esp_err_to_name(err));
    }

    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

/* ==================== 出水/识别后对齐网页（App + 主控都用网页权威值） ==================== */
static void align_with_web(int fp_id)
{
    fetch_user_info_from_web(fp_id);   /* 拉网页权威 → 覆盖本地 daily/use_count/name（含0点重置） */
    update_all_params();               /* 乐鑫 App 参数对齐网页 */
    if (fp_id == current_user_id)
        send_user_info();              /* 主控 USER 命令下发网页权威值 */
}


/* ==================== Voice Broadcast ==================== */
static void voice_broadcast(const char *text)
{
    if (!text || strlen(text) == 0) return;
    char buf[300];
    snprintf(buf, sizeof(buf), "APP_CMD:SPEAK,%s\r\n", text);
    uart_write_bytes(STM32_UART_PORT, buf, strlen(buf));
    ESP_LOGI(TAG, "Voice: %s", text);
}

/* ==================== Daily Water Notification (User-aware) ==================== */
static void notify_daily_water(int dispensed_ml)
{
    user_record_t *u = find_user_by_id(current_user_id);

    int total_today;
    if (u) {
        u->daily_water += dispensed_ml;
        u->total_water += dispensed_ml;
        total_today = u->daily_water;
    } else {
        total_today = dispensed_ml;
    }

    int target = calculate_daily_target(u);
    int remaining = target - total_today;
    if (remaining < 0) remaining = 0;

    char msg[256];
    if (u && strlen(u->name) > 0) {
        snprintf(msg, sizeof(msg),
                 "用户%s，已出水%d毫升，今日共饮水%d毫升，还需饮水%d毫升",
                 u->name, dispensed_ml, total_today, remaining);
    } else if (u) {
        snprintf(msg, sizeof(msg),
                 "用户%d号，已出水%d毫升，今日共饮水%d毫升，还需饮水%d毫升",
                 u->id, dispensed_ml, total_today, remaining);
    } else {
        snprintf(msg, sizeof(msg),
                 "已出水%d毫升，今日已饮水%d毫升，还需饮水%d毫升",
                 dispensed_ml, total_today, remaining);
    }
    voice_broadcast(msg);
    save_users_to_nvs();
    save_users_full();

    update_all_params();

    ESP_LOGI(TAG, "Daily: user=%d today=%d remain=%d", current_user_id, total_today, remaining);

    /* 双发模式：同步上报到本地饮水机网页服务器 */
    send_to_local_server(dispensed_ml, current_user_id);
}

/* ==================== 个性化建议（本地生成，发送给 P4/STM32 统一协议） ==================== */
/* 出水完成后本地计算：下次饮水间隔(分钟)、建议水量(ml)、建议文本 */
static void generate_local_advice(user_record_t *u, int *next_min, int *next_ml, char *text, size_t text_size)
{
    int target = calculate_daily_target(u);
    int daily = u ? u->daily_water : 0;
    int remain = target - daily;
    if (remain <= 0) {
        *next_min = 180;
        *next_ml = 0;
        snprintf(text, text_size, "今日饮水目标已达成，无需再饮");
        return;
    }

    int sip = (remain > 200) ? 200 : remain;   /* 建议每次最多 200ml */
    int times = (remain + sip - 1) / sip;       /* 剩余需饮次数 */

    /* 饮水窗口 07:00-22:00，按当前时间计算剩余可用分钟 */
    time_t now;
    time(&now);
    struct tm *t = localtime(&now);
    int now_min = t->tm_hour * 60 + t->tm_min;
    int window_end = 22 * 60;
    int window = window_end - now_min;
    if (window < 60) window = 60;               /* 至少 1 小时 */

    int interval = window / times;
    if (interval < 30) interval = 30;
    if (interval > 300) interval = 300;

    *next_min = interval;
    *next_ml = sip;
    snprintf(text, text_size, "建议%d分钟后再饮%d毫升", interval, sip);
}

/* 发送个性化建议给主控（STM32/P4 均可），格式：
   APP_CMD:ADVICE,<间隔分钟>,<建议水量ml>,<建议文本>\r\n */
static void send_advice(void)
{
    user_record_t *u = find_user_by_id(current_user_id);
    int next_min = 90, next_ml = 200;
    char text[160];
    generate_local_advice(u, &next_min, &next_ml, text, sizeof(text));

    char buf[256];
    int len = snprintf(buf, sizeof(buf), "APP_CMD:ADVICE,%d,%d,%s\r\n", next_min, next_ml, text);
    uart_write_bytes(STM32_UART_PORT, buf, len);
    ESP_LOGI(TAG, "Advice -> 主控: %s", buf);
}

/* ==================== 用户信息主动下发（C6 自主，无需 App 触发） ==================== */
/* 格式：APP_CMD:USER,<姓名>,<今日饮水量>,<今日使用次数>,<还需饮水>\r\n */
static void send_user_info(void)
{
    user_record_t *u = find_user_by_id(current_user_id);
    char uname[32] = "\xe8\xae\xbf\xe5\xae\xa2";  /* 访客 */
    if (u && u->name[0]) strncpy(uname, u->name, sizeof(uname) - 1);
    int udaily = u ? u->daily_water : 0;
    int ucount = u ? u->use_count : 0;
    int utarget = calculate_daily_target(u);
    int uremain = utarget - udaily;
    if (uremain < 0) uremain = 0;

    char uinf[160];
    int ulen = snprintf(uinf, sizeof(uinf), "APP_CMD:USER,%s,%d,%d,%d\r\n",
                        uname, udaily, ucount, uremain);
    uart_write_bytes(STM32_UART_PORT, uinf, ulen);
    ESP_LOGI(TAG, "UserInfo -> 主控: %s", uinf);
}

/* ==================== DeepSeek 个性化建议（出水后自动生成） ==================== */
/* 触发函数：出水完成后调用，唤醒 advice_worker_task 去调 DeepSeek */
static void generate_advice_and_send(void)
{
    if (advice_sem) {
        xSemaphoreGive(advice_sem);
    } else {
        /* sem 未初始化（启动早期），直接降级本地建议 */
        send_advice();
    }
}

/* DeepSeek worker：带用户上下文调 DeepSeek，解析建议字段，失败降级本地建议 */
static void advice_worker_task(void *arg)
{
    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
    while (1) {
        if (xSemaphoreTake(advice_sem, portMAX_DELAY) == pdTRUE) {
            user_record_t *u = find_user_by_id(current_user_id);
            int target = calculate_daily_target(u);
            int daily = u ? u->daily_water : 0;
            int remain = target - daily;
            if (remain < 0) remain = 0;

            /* 构造 DeepSeek 提问：带用户年龄/身高/体重/今日饮水 */
            char question[400];
            snprintf(question, sizeof(question),
                     "\xe7\x94\xa8\xe6\x88\xb7\xe4\xbf\xa1\xe6\x81\xaf\xef\xbc\x9a\xe6\x80\xa7\xe5\x88\xab%d\xe3\x80\x81\xe5\xb9\xb4\xe9\xbe\x84%d\xe5\xb2\x81\xe3\x80\x81\xe8\xba\xab\xe9\xab\x98%.0f"
                     "cm\xe3\x80\x81\xe4\xbd\x93\xe9\x87\x8d%.0f"
                     "kg\xe3\x80\x81\xe4\xbb\x8a\xe6\x97\xa5\xe5\xb7\xb2\xe9\xa5\xae\xe6\xb0\xb4%d\xe3\x80\x81\xe7\x9b\xae\xe6\xa0\x87\xe9\xa5\xae\xe6\xb0\xb4\xe9\x87\x8f%d"
                     "ml\xe3\x80\x82\xe5\x88\x9a\xe5\xae\x8c\xe6\x88\x90\xe4\xb8\x80\xe6\xac\xa1\xe5\x96\x9d\xe6\xb0\xb4\xe3\x80\x82\xe8\xaf\xb7\xe4\xbb\x85\xe8\xbf\x94\xe5\x9b\x9e\xe4\xb8\x80\xe5\x8f\xa5\xe7\x9f\xad\xe8\xaf\x9d\xef\xbc\x8c\xe5\xbc\x80\xe5\xa4\xb4\xe7\x94\xa8\xe6\x95\xb0\xe5\xad\x97\xe5\x86\x99\xe6\x8e\xa8\xe8\x8d\x90\xe9\x97\xb4\xe9\x9a\x94\xe5\x88\x86\xe9\x92\x9f\xef\xbc\x8c\xe5\x86\x8d\xe5\x86\x99\xe6\x8e\xa8\xe8\x8d\x90\xe6\xb0\xb4\xe9\x87\x8f\xe6\xaf\xab\xe5\x8d\x87\xef\xbc\x8c\xe6\x9c\x80\xe5\x90\x8e\xe5\x86\x99\xe5\xbb\xba\xe8\xae\xae\xe5\x86\x85\xe5\xae\xb9\xef\xbc\x8c\xe5\xbd\xa2\xe5\xbc\x8f\xef\xbc\x9a%%d,%%d,\xe5\xbb\xba\xe8\xae\xae\xe9\x97\xb4\xe9\x9a\x94",
                     u ? u->gender : 0, u ? u->age : 0, u ? u->height_cm : 0, u ? u->weight_kg : 0,
                     daily, target);
            if (remain > 0 && remain < 4000) {
                /* 追加剩余目标让模型参考 */
                strcat(question, "\xe3\x80\x82\xe4\xbb\x8a\xe6\x97\xa5\xe8\xbf\x98\xe9\x9c\x80");
                char tmp[16]; snprintf(tmp, sizeof(tmp), "%d", remain);
                strcat(question, tmp);
                strcat(question, "ml\xe3\x80\x82");
            }
            strcat(question, "\xe4\xbb\x85\xe8\xbf\x94\xe5\x9b\x9e\xe9\x82\xa3\xe4\xb8\x80\xe5\x8f\xa5\xe7\x9f\xad\xe8\xaf\x9d\xef\xbc\x8c\xe4\xb8\x8d\xe5\x8a\xa0\xe8\xa7\xa3\xe9\x87\x8a");

            reply[0] = '\0';
            esp_err_t err = deepseek_query(question, reply, sizeof(reply));
            if (err == ESP_OK && reply[0]) {
                ESP_LOGI(TAG, "DeepSeek advice: %s", reply);
                /* 解析 "分钟,毫升,文本" 格式（DeepSeek 返回形如 90,200,建议90分钟后饮水200毫升） */
                int min = atoi(reply);
                char *p1 = strchr(reply, ',');
                int ml = p1 ? atoi(p1 + 1) : 200;
                char *text = p1 ? strchr(p1 + 1, ',') : NULL;
                char buf[256];
                int len;
                if (text) {
                    len = snprintf(buf, sizeof(buf), "APP_CMD:ADVICE,%d,%d,%s\r\n", min, ml, text + 1);
                } else {
                    len = snprintf(buf, sizeof(buf), "APP_CMD:ADVICE,%d,%d,%s\r\n", min, ml, reply);
                }
                uart_write_bytes(STM32_UART_PORT, buf, len);
                ESP_LOGI(TAG, "Advice -> 主控: %s", buf);
            } else {
                ESP_LOGW(TAG, "DeepSeek advice failed (%s), fallback to local", esp_err_to_name(err));
                send_advice();  /* 降级本地建议 */
            }
        }
    }
    vTaskDelete(NULL);
}

/* ==================== API: Daily Water Stats ==================== */
static esp_err_t api_daily_handler(httpd_req_t *req) {
    user_record_t *u = find_user_by_id(current_user_id);
    int target = calculate_daily_target(u);
    int daily = u ? u->daily_water : 0;
    int remaining = target - daily;
    if (remaining < 0) remaining = 0;
    char buf[192];
    snprintf(buf, sizeof(buf),
             "{\"total\":%d,\"target\":%d,\"remaining\":%d}",
             daily, target, remaining);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, buf, strlen(buf));
    return ESP_OK;
}

/* ==================== API: Users ==================== */
static esp_err_t api_users_handler(httpd_req_t *req) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "current", current_user_id);
    cJSON *jarr = cJSON_AddArrayToObject(root, "users");
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].id == 0) continue;
        cJSON *ju = cJSON_CreateObject();
        cJSON_AddNumberToObject(ju, "id", users[i].id);
        cJSON_AddStringToObject(ju, "name", users[i].name);
        cJSON_AddNumberToObject(ju, "daily", users[i].daily_water);
        cJSON_AddNumberToObject(ju, "total", users[i].total_water);
        cJSON_AddItemToArray(jarr, ju);
    }
    char *out = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, out ? out : "[]", out ? strlen(out) : 2);
    free(out);
    return ESP_OK;
}

/* POST /api/users/set_name  body: {"id":3,"name":"小明"} */
static esp_err_t api_set_user_name_handler(httpd_req_t *req) {
    char buf[256];
    int len = httpd_req_recv(req, buf, 2048 - 1);
    if (len <= 0) { httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "No data"); return ESP_FAIL; }
    buf[len] = '\0';
    cJSON *j = cJSON_Parse(buf);
    if (!j) { httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON"); return ESP_FAIL; }
    cJSON *jid = cJSON_GetObjectItem(j, "id");
    cJSON *jname = cJSON_GetObjectItem(j, "name");
    int fp_id = cJSON_IsNumber(jid) ? jid->valueint : 0;
    const char *name = cJSON_IsString(jname) ? jname->valuestring : "";
    cJSON_Delete(j);
    user_record_t *u = find_user_by_id(fp_id);
    if (u && name && *name) {
        strncpy(u->name, name, sizeof(u->name) - 1);
        save_users_to_nvs();
        httpd_resp_send(req, "{\"ok\":true}", 11);
    } else {
        httpd_resp_send(req, "{\"ok\":false}", 13);
    }
    return ESP_OK;
}

/* POST /api/users/select  body: {"id":3}  — select current user from web */
static esp_err_t api_select_user_handler(httpd_req_t *req) {
    char buf[64];
    int len = httpd_req_recv(req, buf, 2048 - 1);
    if (len <= 0) { httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "No data"); return ESP_FAIL; }
    buf[len] = '\0';
    cJSON *j = cJSON_Parse(buf);
    if (!j) { httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON"); return ESP_FAIL; }
    cJSON *jid = cJSON_GetObjectItem(j, "id");
    int id = cJSON_IsNumber(jid) ? jid->valueint : FP_UNKNOWN;
    cJSON_Delete(j);
    current_user_id = id;
    update_all_params();
    user_record_t *u = find_user_by_id(id);
    char msg[192];
    if (u && strlen(u->name) > 0) {
        snprintf(msg, sizeof(msg), "{\"ok\":true,\"user\":\"%s\"}", u->name);
        char vmsg[128];
        snprintf(vmsg, sizeof(vmsg), "%s，已切换", u->name);
        voice_broadcast(vmsg);
    } else if (u) {
        snprintf(msg, sizeof(msg), "{\"ok\":true,\"user\":\"ID %d\"}", id);
    } else if (id == FP_UNKNOWN) {
        snprintf(msg, sizeof(msg), "{\"ok\":true,\"user\":\"guest\"}");
    } else {
        snprintf(msg, sizeof(msg), "{\"ok\":false}");
    }
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, msg, strlen(msg));
    return ESP_OK;
}

static void dispense_worker_task(void *arg)
{
    dispense_event_t ev;
    while (1) {
        if (xQueueReceive(dispense_queue, &ev, portMAX_DELAY) == pdTRUE) {
            if (ev.set_fp && ev.fp_id >= 0) {
                current_user_id = ev.fp_id;
            }
            if (ev.vol > 0) {
                user_record_t *u = find_user_by_id(current_user_id);
                water_valve_open = false;
                water_volume += ev.vol;
                last_dispensed_volume = ev.vol;
                if (u) u->use_count++;

                /* Call notify (NVS + UART + queues MQTT via work queue) */
                notify_daily_water(ev.vol);
                /* 出水后对齐网页权威值（与真实 STM_DONE 分支一致）：拉回→App→主控USER */
                align_with_web(current_user_id);
            }
        }
    }
    vTaskDelete(NULL);
}

/* ==================== Simulate STM32 Commands (Debug) ==================== */
/* GET /api/sim?fp=1&done=200  — simulate fingerprint + water dispense */
static esp_err_t api_sim_handler(httpd_req_t *req)
{
    char query[128] = {0};
    char fp_str[16] = {0}, done_str[16] = {0};
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        httpd_query_key_value(query, "fp", fp_str, sizeof(fp_str));
        httpd_query_key_value(query, "done", done_str, sizeof(done_str));
    }

    dispense_event_t ev = {0};
    if (strlen(fp_str) > 0) {
        ev.fp_id = atoi(fp_str);
        ev.set_fp = 1;
    }
    if (strlen(done_str) > 0) {
        ev.vol = atoi(done_str);
        /* If no fingerprint set, use current user */
        if (!ev.set_fp) {
            ev.fp_id = current_user_id;
        }
        ev.set_fp = 1;
    }

    if ((ev.set_fp || ev.vol > 0) && dispense_queue) {
        xQueueSend(dispense_queue, &ev, 0);
    }

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, "{\"ok\":true}", 11);
    return ESP_OK;
}

/* ==================== GET /api/ask ==================== */
static esp_err_t api_ask_handler(httpd_req_t *req)
{
    char q[256] = "";
    httpd_req_get_url_query_str(req, q, sizeof(q));
    char *val = strstr(q, "q=");
    if (!val) { httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "?q="); return ESP_FAIL; }
    val += 2;
    for (char *p = val; *p; p++) if (*p == '+') *p = ' ';
    strncpy(deepseek_question, val, sizeof(deepseek_question) - 1);
    deepseek_question[sizeof(deepseek_question) - 1] = '\0';
    xSemaphoreGive(deepseek_sem);
    char *buf = malloc(2048);
    if (!buf) { httpd_resp_send_500(req); return ESP_FAIL; }
    if (xSemaphoreTake(deepseek_done_sem, pdMS_TO_TICKS(30000)) == pdTRUE) {
        snprintf(buf, 2048, "{\"ok\":true,\"reply\":\"%s\"}", deepseek_reply);
    } else {
        snprintf(buf, 2048, "{\"ok\":false,\"error\":\"timeout\"}");
    }
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, buf, strlen(buf));
    return ESP_OK;
}

/* ==================== STM32 UART Task ==================== */
static void stm32_uart_task(void *arg) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_param_config(STM32_UART_PORT, &uart_config);
    uart_set_pin(STM32_UART_PORT, STM32_TX_PIN, STM32_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(STM32_UART_PORT, RX_BUF_SIZE * 2, 0, 0, NULL, 0);

    uint8_t *data = (uint8_t *) malloc(RX_BUF_SIZE);
    ESP_LOGI(TAG, "UART listener ready");

    while (1) {
        int len = uart_read_bytes(STM32_UART_PORT, data, RX_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = '\0';
            char *s = (char *)data;
            /* STM32 data silently processed */
            printf("[UART1] RX: %s\n", s);  // debug
            // 逐行解析
            char *line = s;
            while (line && *line) {
                char *nl = strstr(line, "\n");
                if (nl) *nl = '\0';
                int llen = strlen(line);
                while (llen>0 && (line[llen-1]=='\r'||line[llen-1]==' ')) line[--llen]='\0';
                if (llen==0) { if (nl) line=nl+1; else break; continue; }

            if (strncmp(line, "STM_VOL:", 8) == 0) {
                water_volume = atoi(line + 8);
            } else if (strncmp(line, "STM_DONE:", 9) == 0) {
                water_valve_open = false;
                int done_vol = atoi(line + 9);
                water_volume = done_vol;
                last_dispensed_volume = done_vol;
                if (power_param) esp_rmaker_param_update_and_report(power_param, esp_rmaker_bool(false));
                if (last_vol_param) esp_rmaker_param_update_and_report(last_vol_param, esp_rmaker_int(done_vol));
                user_record_t *done_u = find_user_by_id(current_user_id);
                if (done_u) done_u->use_count++;
                if (count_param) esp_rmaker_param_update_and_report(count_param, esp_rmaker_int(done_u ? done_u->use_count : 0));
                ESP_LOGI(TAG, "Dispense complete: %dml", water_volume);
                notify_daily_water(done_vol);
                /* 出水完成后：①拉网页权威对齐（App + 主控 USER）②自动生成个性化建议（DeepSeek，断网降级本地） */
                align_with_web(current_user_id);
                generate_advice_and_send();
            } else if (strncmp(line, STM_FP_PREFIX, strlen(STM_FP_PREFIX)) == 0) {
                int fp_id = atoi(line + strlen(STM_FP_PREFIX));
                current_user_id = fp_id;
                user_record_t *u = find_user_by_id(fp_id);
                if (u) {
                    char msg[96];
                    if (strlen(u->name) > 0)
                        snprintf(msg, sizeof(msg), "用户%s，请取水", u->name);
                    else
                        snprintf(msg, sizeof(msg), "用户%d号，请取水", fp_id);
                    voice_broadcast(msg);
                    /* 识别流程：从网页拉回权威统计覆盖本地 → App对齐 → 下发主控（只拉不推） */
                    align_with_web(fp_id);
                    ESP_LOGI(TAG, "Fingerprint: user '%s' (ID %d)", u->name ? u->name : "", fp_id);
                } else {
                    ESP_LOGI(TAG, "Fingerprint: unknown ID %d", fp_id);
                }
            } else if (strncmp(line, STM_FP_NEW_PREFIX, strlen(STM_FP_NEW_PREFIX)) == 0) {
                int fp_id = atoi(line + strlen(STM_FP_NEW_PREFIX));
                user_record_t *u = find_user_by_id(fp_id);
                if (!u) {
                    u = add_user(fp_id, NULL);
                }
                if (u) {
                    current_user_id = fp_id;
                    char msg[96];
                    if (strlen(u->name) > 0)
                        snprintf(msg, sizeof(msg), "欢迎%s，请取水", u->name);
                    else
                        snprintf(msg, sizeof(msg), "欢迎新用户，指纹已注册为编号%d，请在网页设置姓名", fp_id);
                    voice_broadcast(msg);
                    /* 识别流程：从网页拉回权威统计覆盖本地 → App对齐 → 下发主控（只拉不推） */
                    align_with_web(fp_id);
                    ESP_LOGI(TAG, "New fingerprint user ID %d", fp_id);
                }
            } else if (strncmp(line, STM_VOICE_PREFIX, strlen(STM_VOICE_PREFIX)) == 0) {
                const char *vname = line + strlen(STM_VOICE_PREFIX);
                char clean[32];
                strncpy(clean, vname, sizeof(clean) - 1);
                clean[sizeof(clean) - 1] = '\0';
                int slen = strlen(clean);
                while (slen > 0 && (clean[slen-1]=='\n'||clean[slen-1]=='\r'||clean[slen-1]==' '))
                    clean[--slen] = '\0';
                user_record_t *u = find_user_by_name(clean);
                if (u) {
                    current_user_id = u->id;
                    char msg[96];
                    snprintf(msg, sizeof(msg), "欢迎%s，请取水", u->name);
                    voice_broadcast(msg);
                    /* 识别流程：从网页拉回权威统计覆盖本地 → App对齐 → 下发主控（只拉不推） */
                    align_with_web(u->id);
                    ESP_LOGI(TAG, "Voice ID: '%s'", u->name);
                } else {
                    static int temp_id_counter = -1;
                    int tid = temp_id_counter--;
                    u = add_user(tid, clean);
                    if (u) {
                        current_user_id = tid;
                        char msg[96];
                        snprintf(msg, sizeof(msg), "已识别新用户%s，请取水", clean);
                        voice_broadcast(msg);
                        /* 新用户（本地临时ID）直接下发本地今日饮水数据给主控 */
                        send_user_info();
                    }
                }
            }
            line = nl ? nl+1 : NULL;
            }
        }
    }
    free(data);
    vTaskDelete(NULL);
}

/* ==================== DeepSeek Worker Task (dedicated stack) ==================== */

static void deepseek_worker_task(void *arg)
{
    while (1) {
        if (xSemaphoreTake(deepseek_sem, portMAX_DELAY) == pdTRUE) {
            deepseek_reply[0] = '\0';
            esp_err_t err = deepseek_query(deepseek_question, deepseek_reply, sizeof(deepseek_reply));
            if (err == ESP_OK && deepseek_reply[0]) {
                printf("\n🤖 DeepSeek: %s\n\n", deepseek_reply);
            } else {
                printf("\n❌ AI Error: %s\n\n", esp_err_to_name(err));
                snprintf(deepseek_reply, sizeof(deepseek_reply), "AI Error: %s", esp_err_to_name(err));
            }
            xSemaphoreGive(deepseek_done_sem);
        }
    }
    vTaskDelete(NULL);
}

/* ==================== Serial Console: Ask DeepSeek ==================== */
/** Type "ask <your question>" in the serial terminal to talk to AI */
static int ask_cmd_handler(int argc, char **argv)
{
    const char *question_start = NULL;

    if (argc >= 2) {
        question_start = argv[1];
    } else if (argc == 1 && strlen(argv[0]) > 3
               && strncmp(argv[0], "ask", 3) == 0) {
        question_start = argv[0] + 3;
    }

    if (!question_start || strlen(question_start) == 0) {
        printf("\nUsage: ask <your question>\n");
        printf("Examples:\n");
        printf("  ask 今天喝多少水比较好？\n");
        printf("  ask 给我倒200ml水\n");
        printf("  ask你好你是谁  (不用空格也可以)\n\n");
        return 0;
    }

    /* Build the question string (no deepseek call here — just store it) */
    char buf[512];
    buf[0] = '\0';
    strcpy(buf, question_start);
    for (int i = 2; i < argc; i++) {
        strcat(buf, " ");
        strcat(buf, argv[i]);
        if (strlen(buf) > 480) break;
    }

    /* Copy to global buffer and signal the worker task */
    if (deepseek_sem) {
        strncpy(deepseek_question, buf, sizeof(deepseek_question) - 1);
        deepseek_question[sizeof(deepseek_question) - 1] = '\0';
        xSemaphoreGive(deepseek_sem);
    }
    return 0;
}

static void day_check_task(void *arg)
{
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(30000));
        check_day_change();
    }
    vTaskDelete(NULL);
}


static void register_ask_command(void)
{
    const esp_console_cmd_t cmd = {
        .command = "ask",
        .help = "Ask DeepSeek AI a question (e.g. ask How much water should I drink)",
        .hint = "<question>",
        .func = &ask_cmd_handler,
    };
    esp_console_cmd_register(&cmd);
    printf("  ask   - Ask DeepSeek AI a question\n");
}

/* ==================== Sim Command ==================== */
static int sim_cmd_handler(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: sim <fp_id> <vol_ml>\n");
        printf("  sim 1 200  - User1 + 200ml\n");
        printf("  sim 0 150  - Guest + 150ml\n");
        return 0;
    }
    int fp_id = atoi(argv[1]);
    int vol = (argc >= 3) ? atoi(argv[2]) : 0;
    if (fp_id > 0) current_user_id = fp_id;
    if (vol > 0 && dispense_queue) {
        dispense_event_t ev = {.vol = vol, .fp_id = current_user_id, .set_fp = 1};
        xQueueSend(dispense_queue, &ev, 0);
    }
    printf(">>> Queued: user %d, %dml\n", fp_id > 0 ? fp_id : 0, vol);
    return 0;
}
static void register_sim_command(void)
{
    const esp_console_cmd_t cmd = {
        .command = "sim",
        .help = "Simulate STM32 commands",
        .hint = "<fp_id> <vol>",
        .func = &sim_cmd_handler,
    };
    esp_console_cmd_register(&cmd);
    printf("  sim   - Simulate STM32 (e.g. sim 1 200)\n");
}

/* ==================== Main ==================== */
void app_main(void)
{
    /* Silence noisy WiFi logs */
    esp_log_level_set("wifi", ESP_LOG_WARN);
    esp_log_level_set("app_wifi", ESP_LOG_ERROR);
    esp_log_level_set("net80211", ESP_LOG_WARN);

    ESP_LOGI(TAG, "=== Smart Water Dispenser (RainMaker) Starting ===");

    /* 1. Initialize NVS */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    /* 2. Initialize RainMaker console (debug commands over UART0) */
    esp_rmaker_console_init();
    register_ask_command();
    register_sim_command();

    /* Load user database from NVS */
    load_users_from_nvs();
    /* Initialize preset 3 users if first boot */
    init_default_users();

    /* Create DeepSeek worker task (large stack for HTTPS/TLS) */
    deepseek_sem = xSemaphoreCreateBinary();
    deepseek_done_sem = xSemaphoreCreateBinary();
    if (deepseek_sem) {
        xTaskCreate(deepseek_worker_task, "deepseek", 24576, NULL, 5, NULL);
    }

    /* Create advice worker task (DeepSeek personalized suggestion after dispense) */
    advice_sem = xSemaphoreCreateBinary();
    if (advice_sem) {
        xTaskCreate(advice_worker_task, "advice", 24576, NULL, 6, NULL);
    }

    /* Create dispense worker queue + task */
    dispense_queue = xQueueCreate(4, sizeof(dispense_event_t));
    if (dispense_queue) {
        xTaskCreate(dispense_worker_task, "disp_worker", 16384, NULL, 8, NULL);
    }

    /* 3. Initialize network (WiFi + BLE provisioning) */
    app_network_init();

    /* 4. Register RainMaker event handler */
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_OTA_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    /* 5. Initialize RainMaker node */
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = true,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, "智能饮水机", "WaterDispenser");
    if (!node) {
        ESP_LOGE(TAG, "Failed to init RainMaker node. Rebooting...");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        esp_restart();
    }

    /* 6. Create Water Dispenser device */
    dispenser_device = esp_rmaker_device_create("饮水机", ESP_RMAKER_DEVICE_SWITCH, NULL);
    esp_rmaker_device_add_cb(dispenser_device, write_cb, NULL);

    /* Name param */
    esp_rmaker_device_add_param(dispenser_device,
        esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, "饮水机"));

    /* Power (on/off) param */
    power_param = esp_rmaker_power_param_create("开关", false);
    esp_rmaker_device_add_param(dispenser_device, power_param);
    esp_rmaker_device_assign_primary_param(dispenser_device, power_param);

    /* Volume param (read-only — shows last dispensed amount from STM32) */
    last_vol_param = esp_rmaker_param_create(
        "上次出水量(ml)", "esp.param.range",
        esp_rmaker_int(0),
        PROP_FLAG_READ);
    esp_rmaker_device_add_param(dispenser_device, last_vol_param);

    /* Today's use count read-only param */
    count_param = esp_rmaker_param_create(
        "今日使用次数", "esp.param.range",
        esp_rmaker_int(0),
        PROP_FLAG_READ);
    esp_rmaker_device_add_param(dispenser_device, count_param);

    /* User ID read-only param */
    user_id_param = esp_rmaker_param_create(
        "用户ID", "esp.param.range",
        esp_rmaker_int(0),
        PROP_FLAG_READ);
    esp_rmaker_device_add_param(dispenser_device, user_id_param);

    /* Remaining water read-only param */
    remain_param = esp_rmaker_param_create(
        "还需饮水(ml)", "esp.param.range",
        esp_rmaker_int(0),
        PROP_FLAG_READ);
    esp_rmaker_param_add_bounds(remain_param, esp_rmaker_int(0), esp_rmaker_int(3000), esp_rmaker_int(10));
    esp_rmaker_device_add_param(dispenser_device, remain_param);

    /* Daily water intake read-only param */
    daily_param = esp_rmaker_param_create(
        "今日饮水", "esp.param.range",
        esp_rmaker_int(0),
        PROP_FLAG_READ);
    esp_rmaker_param_add_bounds(daily_param, esp_rmaker_int(0), esp_rmaker_int(10000), esp_rmaker_int(10));
    esp_rmaker_device_add_param(dispenser_device, daily_param);

    /* 用户信息录入参数（可写，App 填"姓名,年龄,身高,体重" → 存当前用户 NVS → 同步 Web） */
    userinfo_param = esp_rmaker_param_create(
        "\xe7\x94\xa8\xe6\x88\xb7\xe4\xbf\xa1\xe6\x81\xaf\xe5\xbd\x95\xe5\x85\xa5(\xe5\xa7\x93\xe5\x90\x8d,\xe5\xb9\xb4\xe9\xbe\x84,\xe8\xba\xab\xe9\xab\x98,\xe4\xbd\x93\xe9\x87\x8d)", "esp.param.text",
        esp_rmaker_str(""),
        PROP_FLAG_READ | PROP_FLAG_WRITE);
    esp_rmaker_device_add_param(dispenser_device, userinfo_param);

    /* Add device to node */
    esp_rmaker_node_add_device(node, dispenser_device);

    /* 7. Enable optional features */
    esp_rmaker_ota_enable_default();
    esp_rmaker_timezone_service_enable();
    esp_rmaker_schedule_enable();
    esp_rmaker_scenes_enable();

    /* 8. Start RainMaker */
    esp_rmaker_start();

    /* 9. Start WiFi/BLE provisioning via app_network
     *    - First boot: BLE provisioning starts, wait for user via RainMaker app
     *    - Already provisioned: connects to WiFi directly
     *    This blocks until WiFi is connected.
     */
    app_network_set_custom_mfg_data(MFG_DATA_DEVICE_TYPE_SWITCH, MFG_DATA_DEVICE_SUBTYPE_SWITCH);
    /* 使用 MAC 地址作为 PoP（更可靠，无需 fctry 分区预存随机数） */
    err = app_network_start(POP_TYPE_MAC);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Network start failed (%d). Rebooting in 5s...", err);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        esp_restart();
    }

    /* 10. Get device IP */
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (!netif) {
        /* Fallback: try the default station interface */
        netif = esp_netif_get_handle_from_ifkey("STA_DEF");
    }
    if (netif) {
        esp_netif_ip_info_t ip;
        if (esp_netif_get_ip_info(netif, &ip) == ESP_OK) {
            snprintf(device_ip, sizeof(device_ip), IPSTR, IP2STR(&ip.ip));
        }
    }
    ESP_LOGI(TAG, "WiFi connected. IP: %s", device_ip);

    /* 10.5 时间同步（SNTP 独立于 RainMaker 云，保证 0 点重置可靠）
       注意：enable_time_sync=true 时 RainMaker 已在 esp_rmaker_time 里初始化 SNTP，
       这里只能设置服务器与时区，不能再调用 sntp_setoperatingmode（否则触发 lwip 断言重启） */
    esp_sntp_setservername(0, "ntp.aliyun.com");
    esp_sntp_setservername(1, "cn.pool.ntp.org");
    esp_sntp_setservername(2, "pool.ntp.org");
    setenv("TZ", "CST-8", 1);
    tzset();

    /* Check if day changed (reset daily counters) */
    check_day_change();

    /* Periodic day-check task (every 30s, ensures midnight reset) */
    xTaskCreate(day_check_task, "day_check", 2048, NULL, 5, NULL);

    /* 11. Start STM32 UART */
    xTaskCreate(stm32_uart_task, "uart_task", 8192, NULL, 10, NULL);

    /* 12. Start local web dashboard */
    start_web_server();

    /* 12.5 本地服务器同步重试/对账任务（网页服务器没开机时补同步） */
    xTaskCreate(sync_retry_task, "sync_retry", 8192, NULL, 3, NULL);

    /* 13. Initialize user display params */
    update_all_params();

    ESP_LOGI(TAG, "\n==============================================\n"
                  "  Device ready!\n"
                  "  - RainMaker App: use the ESP RainMaker app\n"
                  "  - Local dashboard: http://%s\n"
                  "==============================================", device_ip);
}
