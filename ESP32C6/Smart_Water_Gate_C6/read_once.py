import serial, time, sys, os

# Use mode 2: read from existing boot log if available
# or just open serial without reset
import subprocess
# Read the boot_log.txt that was already saved
log_path = os.path.join(os.path.dirname(__file__) or '.', 'boot_log_60s.txt')
if os.path.exists(log_path):
    with open(log_path, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    for line in content.split('\n'):
        if any(kw in line for kw in ['UART1', 'STM_FP', 'STM_DONE', 'LINE', 'STM_HELLO', 'Dispense', 'Daily']):
            print(line)
    print("\n--- 以上来自历史日志 ---")
else:
    print("No log file found")
