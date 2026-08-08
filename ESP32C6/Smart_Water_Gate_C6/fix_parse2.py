with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# 逐行处理替换
lines = content.split('\n')
new_lines = []
skip = False
in_block = False
block_lines = []

for i, line in enumerate(lines):
    # 先找到if (len > 0) {开始的行
    if 'printf("[UART1] RX' in line:
        # 从这里开始替换直到下一个if分支的结束
        in_block = True
        # 收集当前block
        # 先找到STM_VOL开始的判断行号
        continue

    if in_block:
        block_lines.append(line)
        # 检查是否到了block末尾（下一个完整函数或case）
        if line.strip() == '}' and i+1 < len(lines) and ('if (strncmp(s, "STM_VOL:")' in line or 'if (strncmp(s, "STM_DONE:")' in line):
            continue
        continue

# 换一种更简单的方式：直接查找替换整个代码块
old_marker = 'printf("[UART1] RX: %s\\n", s);  // debug'

idx = content.find(old_marker)
if idx < 0:
    print("NOT FOUND via marker")
    exit(1)

# 找到这一行的行尾
eol = content.find('\n', idx)
# 从这一行开始到下一个平行if的结束
# 找while (1) {的结束位置
search_start = eol

# 找这个if块在哪里结束 - 找STM_FP_NEW或STM_VOICE的else if块结束后的}
end_markers = ['else if (strncmp(s, STM_FP_NEW_PREFIX' ]
# 找到最后一个相关的else if之后到下一个function

# 简单方法：找 "update_all_params();" 的最后一次出现（在UART块中的）
# 然后找到后面的 }

# 直接找 "free(data);" 前面
free_idx = content.find('free(data);', idx)
if free_idx < 0:
    print("free(data) not found")
    exit(1)

# 从printf行到free(data)之间的所有行
block_start = idx
block_end = content.rfind('\n', 0, free_idx) + 1

new_parsing = '''            // 逐行解析（STM32发来的多条命令）
            char *line = s;
            while (line && *line) {
                char *next = strstr(line, "\\\\n");
                if (next) { *next = '\\\\0'; next++; }
                int ln = strlen(line);
                while (ln>0 && (line[ln-1]=='\\\\r'||line[ln-1]=='\\\\n'||line[ln-1]==' ')) line[--ln]='\\\\0';
                if (ln==0) { line=next; continue; }

                if (strncmp(line,"STM_VOL:",8)==0) water_volume=atoi(line+8);
                else if (strncmp(line,"STM_DONE:",9)==0) {
                    water_valve_open=false;
                    int vol=atoi(line+9);
                    water_volume=vol; last_dispensed_volume=vol;
                    user_record_t *du=find_user_by_id(current_user_id);
                    if (du) du->use_count++;
                    notify_daily_water(vol);
                }
                else if (strncmp(line,STM_FP_PREFIX, strlen(STM_FP_PREFIX))==0) {
                    int fid=atoi(line+strlen(STM_FP_PREFIX));
                    current_user_id=fid;
                    user_record_t *u=find_user_by_id(fid);
                    if (u) { voice_broadcast("\\\\u7528\\\\u6237\\\\uff0c\\\\u8bf7\\\\u53d6\\\\u6c34"); update_all_params(); }
                }
                else if (strncmp(line,STM_FP_NEW_PREFIX, strlen(STM_FP_NEW_PREFIX))==0) {
                    int fid=atoi(line+strlen(STM_FP_NEW_PREFIX));
                    user_record_t *u=find_user_by_id(fid);
                    if (!u) u=add_user(fid,NULL);
                    if (u) { current_user_id=fid; voice_broadcast("\\\\u6b22\\\\u8fce\\\\u65b0\\\\u7528\\\\u6237"); update_all_params(); }
                }
                else if (strncmp(line,STM_VOICE_PREFIX, strlen(STM_VOICE_PREFIX))==0) {
                    char nm[32]={0}; strncpy(nm,line+strlen(STM_VOICE_PREFIX),31);
                    user_record_t *u=find_user_by_name(nm);
                    if (u) { current_user_id=u->id; voice_broadcast("\\\\u6b22\\\\u8fce\\\\uff0c\\\\u8bf7\\\\u53d6\\\\u6c34"); update_all_params(); }
                    else { static int t=-1; u=add_user(t--,nm); if (u) current_user_id=u->id; }
                }
                line = next;
            }'''

content = content[:idx] + '            ' + content[idx:]
content = content[:block_start] + new_parsing + content[block_end:]

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
