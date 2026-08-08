with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    lines = f.readlines()

result = []
skip = False
for i, line in enumerate(lines):
    stripped = line.strip()
    if stripped == 'static void update_all_params(void)' and i > 0:
        prev = result[-1].strip() if result else ''
        if prev == 'static void update_all_params(void)':
            skip = True
            continue
    if skip:
        if stripped == '{':
            skip = False
            continue
        if stripped == '}':
            skip = True
            continue
        if stripped.startswith('esp_rmaker') or stripped == '}':
            skip = False
            continue
        continue
    result.append(line)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.writelines(result)
print("DONE")
