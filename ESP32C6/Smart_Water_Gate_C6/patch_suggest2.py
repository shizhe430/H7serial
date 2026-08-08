with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8') as f:
    content = f.read()

# Find the suggest_worker_task function
start = content.find('static void suggest_worker_task(void *arg)')
if start < 0:
    print("ERROR: suggest_worker_task not found")
    exit(1)

# Find the end of the function (next function after it, which is Voice Broadcast)
end = content.find('/* ==================== Voice Broadcast ==================== */', start)
if end < 0:
    print("ERROR: end marker not found")
    exit(1)

print(f"Found suggest_worker_task at {start}, ending at {end}")
old_func = content[start:end]

new_func = '''static void suggest_worker_task(void *arg)
{
    char reply[DEEPSEEK_RESPONSE_MAX_LEN];
    while (1) {
        if (xSemaphoreTake(suggest_sem, portMAX_DELAY) == pdTRUE) {
            reply[0] = '\\0';
            esp_err_t err = deepseek_query(suggest_question, reply, sizeof(reply));
            if (err == ESP_OK && reply[0] && strlen(reply) > 0) {
                printf("[SUGGEST] %s\\n", reply);
            } else {
                printf("[SUGGEST] FAIL: %s\\n", esp_err_to_name(err));
            }
        }
    }
    vTaskDelete(NULL);
}

'''

content = content[:start] + new_func + content[end:]
with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE - removed MQTT update from suggest worker")
