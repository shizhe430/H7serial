import serial, time, sys, os

ser = serial.Serial('COM3', 115200, timeout=3)

# Proper reset
ser.setDTR(False)
ser.setRTS(True)
time.sleep(0.1)
ser.setDTR(True)
time.sleep(0.1)
ser.setRTS(False)
ser.setDTR(False)
time.sleep(1)

start = time.time()
buf = ""
last_line = ""
wifi_connected = False

while time.time() - start < 120:
    try:
        data = ser.read(ser.in_waiting or 1)
        if data:
            text = data.decode('utf-8', errors='replace')
            buf += text
            sys.stdout.write(text)
            sys.stdout.flush()
            last_line += text
            if '\n' in last_line:
                if 'connected with' in last_line:
                    wifi_connected = True
                    print(f"\n*** WiFi CONNECTED at {time.time()-start:.0f}s ***")
                if 'MQTT' in last_line or 'claim' in last_line.lower() and 'success' in last_line.lower():
                    print(f"\n*** CLAIM SUCCESS at {time.time()-start:.0f}s ***")
                last_line = ""
    except:
        pass

ser.close()
print(f"\n=== DONE ({time.time()-start:.0f}s) ===")
with open('boot_log.txt', 'w', encoding='utf-8', errors='replace') as f:
    f.write(buf)
