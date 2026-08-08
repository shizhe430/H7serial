import serial, time, sys, os

SERIAL_PORT = 'COM3'
TIMEOUT = 30

ser = serial.Serial(SERIAL_PORT, 115200, timeout=3)

# Reset the device
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
key_info = []

print(f"Reading from {SERIAL_PORT} for {TIMEOUT}s...")
print("=" * 60)

while time.time() - start < TIMEOUT:
    try:
        data = ser.read(ser.in_waiting or 1)
        if data:
            text = data.decode('utf-8', errors='replace')
            buf += text
            sys.stdout.write(text)
            sys.stdout.flush()
            last_line += text
            if '\n' in last_line:
                line = last_line.rstrip()
                # Capture key info
                lower = line.lower()
                if any(kw in lower for kw in ['pop', 'proof', 'qr code', 'service_name',
                                               'provisioning', 'device', 'connected',
                                               'ip address', 'ready', 'starting',
                                               'rainmaker', 'dispenser']):
                    key_info.append(line)
                last_line = ""
    except:
        pass

ser.close()

print("\n" + "=" * 60)
print("\n*** KEY INFORMATION ***\n")
for line in key_info:
    print(f"  {line}")

# Save full log
log_path = os.path.join(os.path.dirname(__file__) or '.', 'boot_log.txt')
with open(log_path, 'w', encoding='utf-8', errors='replace') as f:
    f.write(buf)
print(f"\nFull log saved to: {log_path}")
