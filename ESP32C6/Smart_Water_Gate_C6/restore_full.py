import re

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Remove duplicate definitions inserted by fix script
# Find the second occurrence of key blocks
for dup in [
    'static SemaphoreHandle_t suggest_sem = NULL;\nstatic char suggest_question[512];\nstatic bool auto_suggest_enabled = true;',
    '/* RainMaker device handles */',
    '/* Dispense worker queue */',
]:
    idx = content.find(dup)
    if idx >= 0:
        idx2 = content.find(dup, idx + len(dup))
        if idx2 >= 0:
            # Find end of this block
            lines = content[idx2:].split('\n')
            end = 1
            for i, l in enumerate(lines):
                if l.startswith('/* ==') or l.startswith('static void ') or l.startswith('static int ') or l.startswith('static esp_err_t ') or l.strip() == '':
                    continue
                end = i
                break
            dup_str = '\n'.join(lines[:max(end,5)])
            if len(dup_str) > 30:
                content = content.replace(dup_str, '', 1)
                print(f"Removed duplicate starting at {idx2}")

# Remove the old dispense_queue definition that conflicts
old_dq = '}\ndispense_event_t;\n\nstatic QueueHandle_t dispense_queue = NULL;\n\n/* ==================== Simulate STM32 Commands (Debug) ==================== */'
new_dq = '} dispense_event_t;\nstatic QueueHandle_t dispense_queue = NULL;\n\n/* ==================== Simulate STM32 Commands (Debug) ==================== */'
content = content.replace(old_dq, new_dq)

# Add forward declarations for missing functions
# insert before the globals block
missing_decls = '''
/* Forward declarations for missing functions */
static void save_users_full(void);
static void load_users_from_nvs(void);
static void init_default_users(void);
static void check_day_change(void);
static void start_web_server(void);
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
                           const esp_rmaker_param_val_t val, void *priv_data,
                           esp_rmaker_write_ctx_t *ctx);
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data);
static void register_ask_command(void);
'''

# Add after the existing forward declarations
idx = content.find('static void save_users_to_nvs(void);')
if idx >= 0:
    next_line = content.find('\n', idx)
    content = content[:next_line+1] + missing_decls + content[next_line+1:]
    print("Added forward declarations")
else:
    print("ERROR: could not find anchor point")

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE - file written")
