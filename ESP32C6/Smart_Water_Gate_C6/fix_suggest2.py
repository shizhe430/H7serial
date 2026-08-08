with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# First add the include, cache var, and callback function before suggest_worker_task
marker = 'static void suggest_worker_task(void *arg)'
insert = '''static char suggest_cache[256] = "";

static void _update_suggest_param(void *priv)
{
    if (suggest_param && strlen(suggest_cache) > 0) {
        esp_rmaker_param_update_and_report(suggest_param, esp_rmaker_str(suggest_cache));
    }
}

static void suggest_worker_task(void *arg)'''

content = content.replace(marker, insert, 1)

# Now replace the function body
old_body = '''    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
    while (1) {
        if (xSemaphoreTake(suggest_sem, portMAX_DELAY) == pdTRUE) {
            reply[0] = '\\0';
            esp_err_t err = deepseek_query(suggest_question, reply, sizeof(reply));
            if (err == ESP_OK && reply[0]) {
                if (suggest_param) {
                    esp_rmaker_param_update_and_report(suggest_param, esp_rmaker_str(reply));
                }
                printf("[SUGGEST] OK\\n");
            } else {
                printf("[SUGGEST] FAIL: %s\\n", esp_err_to_name(err));
            }
        }
    }
    vTaskDelete(NULL);'''

new_body = '''    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
    while (1) {
        if (xSemaphoreTake(suggest_sem, portMAX_DELAY) == pdTRUE) {
            reply[0] = '\\0';
            esp_err_t err = deepseek_query(suggest_question, reply, sizeof(reply));
            if (err == ESP_OK && reply[0]) {
                strncpy(suggest_cache, reply, sizeof(suggest_cache) - 1);
                suggest_cache[sizeof(suggest_cache) - 1] = '\\0';
                esp_rmaker_work_queue_add_task(_update_suggest_param, NULL);
                printf("[SUGGEST] OK\\n");
            } else {
                printf("[SUGGEST] FAIL: %s\\n", esp_err_to_name(err));
            }
        }
    }
    vTaskDelete(NULL);'''

content = content.replace(old_body, new_body, 1)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
