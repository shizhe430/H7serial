# Add ALL missing global variables after the existing globals
with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Find the position after forward declarations to insert globals
marker = 'static SemaphoreHandle_t suggest_sem = NULL;\nstatic char suggest_question[512];\nstatic bool auto_suggest_enabled = true;\n\nstatic void suggest_worker_task(void *arg)'
replacement = '''static SemaphoreHandle_t suggest_sem = NULL;
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
static esp_rmaker_param_t *trigger_param = NULL;
static esp_rmaker_param_t *suggest_param = NULL;

/* HTTP server */
static httpd_handle_t server = NULL;
static char device_ip[16] = "0.0.0.0";

/* Dispense worker queue */
typedef struct {
    int vol;
    int fp_id;
    int set_fp;
} dispense_event_t;
static QueueHandle_t dispense_queue = NULL;

static void suggest_worker_task(void *arg)'''

content = content.replace(marker, replacement)
with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("ADDED all missing globals")
