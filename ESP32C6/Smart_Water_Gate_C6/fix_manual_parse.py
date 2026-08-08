with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# The problem: strtok may not work reliably. Let's replace with manual line-by-line parsing.

# Replace the strtok-based parsing with manual line parsing
old = '''            // 逐行解析
            char *tok = strtok(s, "\\r\\n");
            while (tok) {

            if (strncmp(tok, "STM_VOL:", 8) == 0) {'''

new = '''            // 逐行解析
            char *line = s;
            while (line && *line) {
                char *nl = strstr(line, "\\n");
                if (nl) *nl = '\\0';
                int llen = strlen(line);
                while (llen>0 && (line[llen-1]=='\\r'||line[llen-1]==' ')) line[--llen]='\\0';
                if (llen==0) { if (nl) line=nl+1; else break; continue; }

            if (strncmp(line, "STM_VOL:", 8) == 0) {'''

content = content.replace(old, new)
print(f"strtok replaced: {'YES' if old in content else 'DONE'}")

# Replace tok references with line references where needed
for old_ref, new_ref in [
    ('strncmp(tok, "STM_VOL:', 'strncmp(line, "STM_VOL:'),
    ('strncmp(tok, "STM_DONE:', 'strncmp(line, "STM_DONE:'),
    ('strncmp(tok, STM_FP_PREFIX', 'strncmp(line, STM_FP_PREFIX'),
    ('strncmp(tok, STM_FP_NEW_PREFIX', 'strncmp(line, STM_FP_NEW_PREFIX'),
    ('strncmp(tok, STM_VOICE_PREFIX', 'strncmp(line, STM_VOICE_PREFIX'),
    ('atoi(tok + 8)', 'atoi(line + 8)'),
    ('atoi(tok + 9)', 'atoi(line + 9)'),
    ('atoi(tok + strlen(STM_FP_PREFIX))', 'atoi(line + strlen(STM_FP_PREFIX))'),
    ('atoi(tok + strlen(STM_FP_NEW_PREFIX))', 'atoi(line + strlen(STM_FP_NEW_PREFIX))'),
    ('const char *vname = tok + strlen(STM_VOICE_PREFIX)', 'const char *vname = line + strlen(STM_VOICE_PREFIX)'),
    ('tok = strtok(NULL', 'line = nl ? nl+1 : NULL'),
]:
    if old_ref in content:
        content = content.replace(old_ref, new_ref)
        print(f"  Replaced: {old_ref[:40]}...")

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("\nDONE")
