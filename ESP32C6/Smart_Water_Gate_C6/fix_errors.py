with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# 1. Fix the suggest_question format string in write_cb - replace with simple version
old_q = '''snprintf(suggest_question, sizeof(suggest_question),
                     "\\xe7\\x94\\xa8\\xe6\\x88\\xb7%s\\xef\\xbc\\x8c\\xe6\\x80\\xa7\\xe5\\x88\\xab%d\\xef\\xbc\\x8c\\xe5\\xb9\\xb4\\xe9\\xbe\\x84%d\\xef\\xbc\\x8c\\xe8\\xba\\xab\\xe9\\xab\\x98\\xe2\\x80\\x94\\xe2\\x80\\x942fcm\\xef\\xbc\\x8c\\xe4\\xbd\\x93\\xe9\\x87\\x8d\\xe2\\x80\\x94\\xe2\\x80\\x942fkg\\xef\\xbc\\x8c\\xe4\\xbb\\x8a\\xe6\\x97\\xa5\\xe5\\xb7\\xb2\\xe9\\xa5\\xae\\xe6\\xb0\\xb4%dml\\xef\\xbc\\x8c\\xe5\\xbb\\xba\\xae\\xe8\\xae\\xae\\xe7\\x9b\\xae\\xe6\\xa0\\x87%dml\\xe3\\x80\\x82\\xe8\\xaf\\xb7\\xe7\\xbb\\x99\\xe5\\x87\\xba\\xe4\\xb8\\x80\\xe6\\x9d\\xa1\\xe7\\xae\\x80\\xe7\\x9f\\xad\\xe4\\xb8\\xaa\\xe6\\x80\\xa7\\xe5\\x8c\\x96\\xe7\\x9a\\x84\\xe9\\xa5\\xae\\xe6\\xb0\\xb4\\xe5\\xbb\\xba\\xe8\\xae\\xae\\xef\\xbc\\x8850\\xe5\\xad\\x97\\xe4\\xbb\\xa5\\xe5\\x86\\x85\\xef\\xbc\\x89\\xe3\\x80\\x82",
                     uname, u ? u->gender : 0, u ? u->age : 0,
                     u ? u->height_cm : 0, u ? u->weight_kg : 0,
                     daily, target);'''

new_q = '''snprintf(suggest_question, sizeof(suggest_question),
                     "User %s, gender %d, age %d, height %.0f cm, weight %.0f kg, drank %d ml today, target %d ml. Give short advice (<50 chars).",
                     uname, u ? u->gender : 0, u ? u->age : 0,
                     (double)(u ? u->height_cm : 0), (double)(u ? u->weight_kg : 0),
                     daily, target);'''

content = content.replace(old_q, new_q)
print("Fixed suggest_question format")

# 2. Add forward declarations for API handlers before start_web_server
old_ws = '''static void start_web_server(void) {'''
new_ws = '''static esp_err_t api_daily_handler(httpd_req_t *req);
static esp_err_t api_users_handler(httpd_req_t *req);
static esp_err_t api_set_user_name_handler(httpd_req_t *req);
static esp_err_t api_select_user_handler(httpd_req_t *req);
static esp_err_t api_sim_handler(httpd_req_t *req);
static esp_err_t api_ask_handler(httpd_req_t *req);

static void start_web_server(void) {'''

content = content.replace(old_ws, new_ws)
print("Fixed forward declarations")

# 3. Remove duplicate dispense_queue/event_t (again)
old_dq2 = '''static QueueHandle_t dispense_queue = NULL;
typedef struct {
    int vol;
    int fp_id;
    int set_fp;
} dispense_event_t;
static QueueHandle_t dispense_queue = NULL;
'''
new_dq2 = '''static QueueHandle_t dispense_queue = NULL;
typedef struct {
    int vol;
    int fp_id;
    int set_fp;
} dispense_event_t;
'''
content = content.replace(old_dq2, new_dq2)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
