# Add back missing global vars and function implementations
with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Add suggest globals before suggest_worker_task
old = 'static void suggest_worker_task(void *arg)'
new = 'static SemaphoreHandle_t suggest_sem = NULL;\nstatic char suggest_question[512];\nstatic bool auto_suggest_enabled = true;\n\nstatic void suggest_worker_task(void *arg)'

if old in content:
    content = content.replace(old, new, 1)
    with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("ADDED globals")
else:
    print("NOT FOUND")
    idx = content.find('suggest_worker_task')
    if idx > 0:
        print(repr(content[idx-100:idx+100]))
