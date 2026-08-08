with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Replace update_all_params to use work queue
old = '''static void update_all_params(void)
{
    _rmaker_update(NULL);
}'''

new = '''static void update_all_params(void)
{
    esp_rmaker_work_queue_add_task(_rmaker_update, NULL);
}'''

if old in content:
    content = content.replace(old, new)
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("DONE - use work queue")
else:
    print("NOT FOUND")
