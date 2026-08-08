import serial, time, sys, os

ser = serial.Serial('COM3', 115200, timeout=3)
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

print(f"Reading for 25s...")

while time.time() - start < 25:
    try:
        data = ser.read(ser.in_waiting or 1)
        if data:
            text = data.decode('utf-8', errors='replace')
            buf += text
            sys.stdout.write(text)
            sys.stdout.flush()
    except:
        pass

ser.close()

# Save log
with open('boot_log.txt', 'w', encoding='utf-8', errors='replace') as f:
    f.write(buf)
print("\n\nDone")
