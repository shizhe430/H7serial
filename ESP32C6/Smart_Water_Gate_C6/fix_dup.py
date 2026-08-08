with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Remove the SECOND occurrence of dispense_queue definition
old = '''/* ==================== Dispense Worker (async processing) ==================== */
static QueueHandle_t dispense_queue = NULL;
typedef struct {
    int vol;
    int fp_id;
    int set_fp; /* 1 = also set fingerprint */
} dispense_event_t;

static void dispense_worker_task(void *arg)'''

new = '''static void dispense_worker_task(void *arg)'''

content = content.replace(old, new, 1)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("REMOVED duplicate")
