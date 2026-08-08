with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Fix the broken printf line - replace broken line with correct one
old_broken = '            printf("[UART1] %s\n", s);'
correct = '            printf("[UART1] RX: %s\\n", s);  // debug'

content = content.replace(old_broken, correct)

# If the \n was stored as actual newline, remove the broken line
for broken in [
    '            printf("[UART1] %s\n", s);',
    '            printf("[UART1] %s\n", s);'
]:
    content = content.replace(broken, correct)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
