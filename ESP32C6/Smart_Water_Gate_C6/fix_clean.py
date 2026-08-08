with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Remove everything from the broken sed lines (line 790-798)
import re
# Find the broken section
marker = 'printf("\\[UART1\\] RX: %s\\n", s);  // debug'
idx = content.find(marker)
if idx < 0:
    print("MARKER NOT FOUND")
    exit(1)

eol = content.find('\n', idx)
start_replace = eol  # first char after the printf line

# Find the end of the broken lines - find "if (strncmp(s, \"STM_VOL\""
end_replace = content.find('if (strncmp(s, "STM_VOL:', 8) == 0)', start_replace)
if end_replace < 0:
    print("END NOT FOUND")
    exit(1)

# Also remove trailing spaces/newlines before the if
# remove the broken while loop lines
broken_section = content[start_replace:end_replace]
print(f"Removing {len(broken_section)} bytes of broken code")
content = content[:start_replace] + '\n' + content[end_replace:]

# Now replace all 'if (strncmp(s, "STM_' with 'if (strncmp(tok, "STM_'
# and all 'atoi(s +' with 'atoi(tok +'
content = content.replace('if (strncmp(s, "STM_VOL:', 8) == 0)', 'if (strncmp(tok, "STM_VOL:", 8) == 0)')
content = content.replace('if (strncmp(s, "STM_DONE:', 9) == 0)', 'if (strncmp(tok, "STM_DONE:", 9) == 0)')
content = content.replace('if (strncmp(s, STM_FP_PREFIX, strlen(STM_FP_PREFIX)) == 0)', 'if (strncmp(tok, STM_FP_PREFIX, strlen(STM_FP_PREFIX)) == 0)')
content = content.replace('if (strncmp(s, STM_FP_NEW_PREFIX, strlen(STM_FP_NEW_PREFIX)) == 0)', 'if (strncmp(tok, STM_FP_NEW_PREFIX, strlen(STM_FP_NEW_PREFIX)) == 0)')
content = content.replace('if (strncmp(s, STM_VOICE_PREFIX, strlen(STM_VOICE_PREFIX)) == 0)', 'if (strncmp(tok, STM_VOICE_PREFIX, strlen(STM_VOICE_PREFIX)) == 0)')
# Replace atoi(s + with atoi(tok +
content = content.replace('atoi(s + 8)', 'atoi(tok + 8)')
content = content.replace('atoi(s + 9)', 'atoi(tok + 9)')
content = content.replace('atoi(s + strlen(STM_FP_PREFIX))', 'atoi(tok + strlen(STM_FP_PREFIX))')
content = content.replace('atoi(s + strlen(STM_FP_NEW_PREFIX))', 'atoi(tok + strlen(STM_FP_NEW_PREFIX))')
content = content.replace('const char *vname = s + strlen(STM_VOICE_PREFIX)', 'const char *vname = tok + strlen(STM_VOICE_PREFIX)')

# Remove the broken while-loop lines that may remain
content = re.sub(r'char \*line = s;.*?while \(line && \*line\) \{.*?\n.*?\n.*?\n.*?\n.*?\n', '', content, flags=re.DOTALL)
# Remove any leftover stray characters like standalone '
content = content.replace("\n            while (line && *line) {\n", "\n")
content = content.replace("char *nl = strstr(line, \"\n", "")

# Add the strtok loop
# Find where to insert the loop start (after printf line)
idx2 = content.find(marker)
eol2 = content.find('\n', idx2)
loop_start = marker + '\n'
loop_start += '            // 逐行解析\n'
loop_start += '            char *tok = strtok(s, "\\r\\n");\n'
loop_start += '            while (tok) {\n'

# Now we need to find the closing brace of the if-else chain
# It's before 'free(data);'
free_idx = content.find('\n    free(data);')
# Go backward to find the matching closing brace
close_brace = content.rfind('\n            }', idx2, free_idx)
if close_brace > 0:
    # Insert loop end before that }
    content = content[:close_brace] + '\n                tok = strtok(NULL, "\\r\\n");\n            }' + content[close_brace:]

content = content.replace(loop_start, loop_start)

content = content.replace(marker, loop_start.strip())

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
