with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    text = f.read()

# Remove the broken lines from sed (lines with actual newlines in string)
text = text.replace('\n            while (line && *line) {\n', '\n')
text = text.replace('char *nl = strstr(line, "\n", 1);', '')
text = text.replace('\n                if (nl) { *nl = 0; nl++; }\n', '\n')
text = text.replace('\n                int ll = strlen(line);\n                while (ll>0 && (line[ll-1]==\'\r\'||line[ll-1]==\'\n\'||line[ll-1]==\' \')) line[--ll]=0;\n                if (ll==0) { line=nl; continue; }\n', '\n')

# Now add the correct loop wrapper
# Find the printf line
marker = 'printf("[UART1] RX: %s\\n", s);  // debug\n'
insert = '''printf("[UART1] RX: %s\\n", s);  // debug
            // 逐行解析
            char *tok = strtok(s, "\\r\\n");
            while (tok) {
                printf("[UART1] LINE: %s\\n", tok);
'''

end_marker = '            }'
# Replace the first if (strncmp... occurrences with the loop wrapper

# Actually, simpler: find the first if statement after printf and wrap
after_printf = text.find(insert)
if after_printf < 0:
    print("marker not found")
    exit(1)

# Find the first "if (strncmp" after printf
first_if = text.find('if (strncmp', after_printf)
# Find the closing brace of the whole if-else chain
# The chain ends before "free(data)"
free_idx = text.find('free(data);', first_if)

if first_if < 0 or free_idx < 0:
    print("Cant find boundaries")
    exit(1)

# Insert the loop start before the first if
loop_start = '"逐行解析\\n";\n'
# Actually let me just do a simple string replacement
# Replace the line after printf with loop wrapper + first if start

old = text[after_printf:after_printf + len(insert)]
new = '''printf("[UART1] RX: %s\\n", s);  // debug
            // 逐行解析
            char *tok = strtok(s, "\\r\\n");
            while (tok) {
                printf("[UART1] LINE: %s\\n", tok);
'''

text = text[:after_printf] + new + text[after_printf + len(insert):]

# Find where the parsing block ends (before vTaskDelete)
# The last else if in the chain - after it there's 'free(data)'
free_idx = text.find('free(data);', first_if + 100)
if free_idx > 0:
    # Find the closing brace before free
    close_idx = text.rfind('\\n            }\\n', first_if, free_idx)
    if close_idx > 0:
        # Insert while loop end
        text = text[:close_idx] + '            }  // end while tok\\n' + text[close_idx:]

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(text)
print("DONE")
