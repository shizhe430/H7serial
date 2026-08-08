with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Fix the messed up line 789
# Find the broken printf line and replace it
old = '            printf("[UART1] %d bytes: ", len); for(int i=0;i<len            printf("[UART1] %d bytes: ", len); for(int i=0;i<len            printf("[UART1] RX: %s\\n", s);  // debug            printf("[UART1] RX: %s\\n", s);  // debugi<20;i++) printf("%02x ",(unsigned char)data[i]); printf("            printf("[UART1] %d bytes: ", len); for(int i=0;i<len            printf("[UART1] RX: %s\\n", s);  // debug            printf("[UART1] RX: %s\\n", s);  // debugi<20;i++) printf("%02x ",(unsigned char)data[i]); printf("i<20;i++) printf("%02x ",(unsigned char)data[i]); printf('

# Actually, let's just find the specific pattern and fix it
import re
# Find the whole broken line
pattern = r'printf\("\[UART1\].*'
matches = re.findall(pattern, content)
for m in matches:
    if 'bytes' in m:
        content = content.replace(m, '            printf("[UART1] RX: %s\\n", s);  // debug')
        print("Fixed broken line")
        break

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
