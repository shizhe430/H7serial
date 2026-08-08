# Insert ALL missing function implementations before suggest_worker_task
with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

block = r'''
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
    nvs_close(h);
}

static void init_default_users(void)
{
    if (find_user_by_id(1)) return;
    static const struct {
        int id; const char *name; int gender; int age; float h; float w;
    } preset[] = {
        {1, "\xe7\x94\xa8\xe6\x88\xb71", 1, 20, 178, 67},
        {2, "\xe7\x94\xa8\xe6\x88\xb72", 1, 20, 181, 78},
        {3, "\xe7\x94\xa8\xe6\x88\xb73", 1, 20, 174, 60},
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
    if (!u || u->age <= 0) return 1700;
    float bmr;
    if (u->gender == 1) {
        bmr = 10 * u->weight_kg + 6.25 * u->height_cm - 5 * u->age + 5;
    } else if (u->gender == 2) {
        bmr = 10 * u->weight_kg + 6.25 * u->height_cm - 5 * u->age - 161;
    } else {
        return 1700;
    }
    int water = (int)(bmr * 0.35);
    if (water < 1000) water = 1000;
    if (water > 3000) water = 3000;
    return water;
}

static void update_all_params(void)
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

/* ==================== RainMaker Write Callback ==================== */
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                           const esp_rmaker_param_val_t val, void *priv_data,
                           esp_rmaker_write_ctx_t *ctx)
{
    if (strcmp(esp_rmaker_param_get_name(param), "\xe5\xbc\x80\xe5\x85\xb3") == 0) {
        if (val.val.b) {
            water_valve_open = true;
            uart_write_bytes(STM32_UART_PORT, "APP_CMD:START\r\n", 15);
        } else {
            water_valve_open = false;
            uart_write_bytes(STM32_UART_PORT, "APP_CMD:STOP\r\n", 14);
        }
        esp_rmaker_param_update_and_report(param, val);
    } else if (strcmp(esp_rmaker_param_get_name(param), "\xe8\x8e\xb7\xe5\x8f\x96\xe5\xbb\xba\xe8\xae\xae") == 0) {
        auto_suggest_enabled = val.val.b;
        if (val.val.b) {
            user_record_t *u = find_user_by_id(current_user_id);
            int daily = u ? u->daily_water : 0;
            int target = calculate_daily_target(u);
            char uname[32] = "\xe8\xae\xbf\xe5\xae\xa2";
            if (u && strlen(u->name) > 0) snprintf(uname, sizeof(uname), "%s", u->name);
            else if (u) snprintf(uname, sizeof(uname), "\xe7\x94\xa8\xe6\x88\xb7%d", u->id);
            snprintf(suggest_question, sizeof(suggest_question),
                     "\xe7\x94\xa8\xe6\x88\xb7%s\xef\xbc\x8c\xe6\x80\xa7\xe5\x88\xab%d\xef\xbc\x8c\xe5\xb9\xb4\xe9\xbe\x84%d\xef\xbc\x8c\xe8\xba\xab\xe9\xab\x98\xe2\x80\x94\xe2\x80\x942fcm\xef\xbc\x8c\xe4\xbd\x93\xe9\x87\x8d\xe2\x80\x94\xe2\x80\x942fkg\xef\xbc\x8c\xe4\xbb\x8a\xe6\x97\xa5\xe5\xb7\xb2\xe9\xa5\xae\xe6\xb0\xb4%dml\xef\xbc\x8c\xe5\xbb\xba\xe8\xae\xae\xe7\x9b\xae\xe6\xa0\x87%dml\xe3\x80\x82\xe8\xaf\xb7\xe7\xbb\x99\xe5\x87\xba\xe4\xb8\x80\xe6\x9d\xa1\xe7\xae\x80\xe7\x9f\xad\xe4\xb8\xaa\xe6\x80\xa7\xe5\x8c\x96\xe7\x9a\x84\xe9\xa5\xae\xe6\xb0\xb4\xe5\xbb\xba\xe8\xae\xae\xef\xbc\x8850\xe5\xad\x97\xe4\xbb\xa5\xe5\x86\x85\xef\xbc\x89\xe3\x80\x82",
                     uname, u ? u->gender : 0, u ? u->age : 0,
                     u ? u->height_cm : 0, u ? u->weight_kg : 0,
                     daily, target);
            xSemaphoreGive(suggest_sem);
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
'''

# Insert before suggest_worker_task
marker = 'static void suggest_worker_task(void *arg)'
content = content.replace(marker, block + '\n' + marker, 1)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("INSERTED all missing functions")
