with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

old = '''static void suggest_worker_task(void *arg)
{
    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
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
    vTaskDelete(NULL);
}

/* ==================== Voice Broadcast ==================== */'''

new = '''static void suggest_worker_task(void *arg)
{
    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
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
    vTaskDelete(NULL);
}

/* ==================== Voice Broadcast ==================== */'''

if old in content:
    content = content.replace(old, new)
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("REPLACED")
else:
    print("NOT FOUND")
    idx = content.find('suggest_worker_task')
    if idx > 0:
        print(repr(content[idx:idx+400]))
