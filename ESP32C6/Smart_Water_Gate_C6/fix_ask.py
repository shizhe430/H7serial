with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

idx = content.find('char buf[6000];')
if idx > 0:
    # Replace char buf[6000] with malloc
    content = content[:idx] + 'char *buf = malloc(2048);\n    if (!buf) { httpd_resp_send_500(req); return ESP_FAIL; }' + content[idx+13:]
    # Replace sizeof(buf) with 2048
    content = content.replace('sizeof(buf)', '2048')
    # Add free(buf) before return
    content = content.replace(
        'httpd_resp_send(req, buf, strlen(buf));\n    return ESP_OK;}',
        'httpd_resp_send(req, buf, strlen(buf));\n    free(buf);\n    return ESP_OK;}',
        1
    )
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("FIXED")
else:
    print("NOT FOUND")
