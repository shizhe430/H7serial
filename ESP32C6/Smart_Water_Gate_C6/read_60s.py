import serial, time, sys
ser = serial.Serial('COM3', 115200, timeout=1)
time.sleep(2)

start = time.time()
buf = ""
print("Reading for 45 seconds...")
while time.time() - start < 45:
    try:
        data = ser.read(ser.in_waiting or 0.5)
        if data:
            text = data.decode('utf-8', errors='replace')
            buf += text
            if 'UART1' in text:
                sys.stdout.write("\n[抓到] " + text.strip() + "\n")
            elif 'STM32' in text or 'STM_FP' in text or 'STM_DONE' in text:
                sys.stdout.write("\n[抓到] " + text.strip() + "\n")
    except:
        pass
ser.close()
print("\n完成")
with open('boot_log_60s.txt','w',encoding='utf-8',errors='replace') as f: f.write(buf)
