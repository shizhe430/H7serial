import re

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8') as f:
    content = f.read()

old = '''static void suggest_worker_task(void *arg)
{
    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
    while (1) {
        if (xSemaphoreTake(suggest_sem, portMAX_DELAY) == pdTRUE) {
            reply[0] = '\\0';
            esp_err_t err = deepseek_query(suggest_question, reply, sizeof(reply));
            if (err == ESP_OK && reply[0]) {
                ESP_LOGI(TAG, "Suggestion: %.60s", reply);
                if (suggest_param) esp_rmaker_param_update_and_report(suggest_param, esp_rmaker_str(reply));
            } else {
                ESP_LOGW(TAG, "Suggestion failed: %s", esp_err_to_name(err));
            }
        }
    }
    vTaskDelete(NULL);
}'''

new = '''static void suggest_worker_task(void *arg)
{
    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
    while (1) {
        if (xSemaphoreTake(suggest_sem, portMAX_DELAY) == pdTRUE) {
            printf("[SUGGEST] Asking DeepSeek...\\n");
            reply[0] = '\\0';
            esp_err_t err = deepseek_query(suggest_question, reply, sizeof(reply));
            printf("[SUGGEST] err=%d reply='%.80s'\\n", err, reply);
            if (err == ESP_OK && reply[0]) {
                if (suggest_param) {
                    printf("[SUGGEST] Updating param...\\n");
                    esp_rmaker_param_update_and_report(suggest_param, esp_rmaker_str(reply));
                    printf("[SUGGEST] Done!\\n");
                }
            } else {
                printf("[SUGGEST] Failed\\n");
            }
        }
    }
    vTaskDelete(NULL);
}'''

if old in content:
    content = content.replace(old, new)
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("REPLACED SUCCESS")
else:
    print("NOT FOUND")
    idx = content.find("suggest_worker_task")
    if idx > 0:
        print(repr(content[idx:idx+600]))
