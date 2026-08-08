with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    lines = f.readlines()

# Fix line 789 (0-indexed: 788)
for i, line in enumerate(lines):
    if 'printf("[UART1]' in line:
        lines[i] = '            printf("[UART1] RX: %s\\n", s);  // debug\n'
        print(f"Fixed line {i+1}: {lines[i].strip()}")
        break

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.writelines(lines)
print("DONE")
