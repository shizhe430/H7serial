with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'r', encoding='utf-8', errors='replace') as f:
    content = f.read()

# Remove the wrong gpio pullup
content = content.replace("    gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);\n", "")

# Add uart_flush after console init
content = content.replace(
    "    esp_rmaker_console_init();\n    register_ask_command();",
    "    esp_rmaker_console_init();\n    uart_flush(UART_NUM_0);\n    register_ask_command();"
)

with open(r'C:\Users\17360\workspace\Smart_Water_Gate_C6\main\main.c', 'w', encoding='utf-8') as f:
    f.write(content)
print("DONE")
