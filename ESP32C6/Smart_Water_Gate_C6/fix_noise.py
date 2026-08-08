with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# 1. Delete the ESP_LOGI line that prints STM32 data (causes console flood)
content = content.replace(
    '            ESP_LOGI(TAG, "STM32: %s", s);\n',
    '            /* STM32 data silently processed */\n'
)

# 2. Add RX pin pull-up config before console init
old = '    esp_rmaker_console_init();'
new = '''    /* Set UART RX pin pull-up to prevent floating noise */
    gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);
    esp_rmaker_console_init();'''

content = content.replace(old, new)

# 3. Ensure gpio header is included
old = '#include "driver/gpio.h"'
new = '#include "driver/gpio.h"\n#include "hal/gpio_hal.h"'

# Only add if not already there
if not 'hal/gpio_hal.h' in content:
    content = content.replace(old, new)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
