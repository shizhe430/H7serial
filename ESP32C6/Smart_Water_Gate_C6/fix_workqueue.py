with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# 找到第一个 static void update_all_params(void) 及其实现
idx1 = content.find('static void update_all_params(void)')
if idx1 >= 0:
    # 找函数体开始
    brace = content.find('{', idx1)
    if brace >= 0:
        brace_end = content.find('}', brace)
        if brace_end >= 0:
            # 检查是否有第二个重复
            rest = content[brace_end+1:]
            idx2 = rest.find('static void update_all_params(void)')
            if idx2 >= 0 and idx2 < 50:  # 很近就有重复
                close2 = rest.find('}', idx2)
                if close2 >= 0:
                    # 删除后面的重复内容
                    content = content[:brace_end+1] + rest[close2+1:]
                    print("Removed duplicate")

# 确保函数体是 work queue 方式
old_func = '''static void update_all_params(void)
{
    _rmaker_update(NULL);
}'''
new_func = '''static void update_all_params(void)
{
    esp_rmaker_work_queue_add_task(_rmaker_update, NULL);
}'''
content = content.replace(old_func, new_func)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
