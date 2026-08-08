import serial, time, sys

# ========== ESP32-C6 Serial WiFi Config Tool ==========
# Sends WIFI:SSID,PASSWORD command over USB serial
#
# The ESP32 listens on USB serial for: WIFI:SSID,PASSWORD
# Just connect to the serial port and send the command.

if len(sys.argv) < 3:
    print("Usage: python wifi_config.py COM3 <SSID> <Password>")
    print()
    if len(sys.argv) >= 2:
        port = sys.argv[1]
        ssid = input("Enter WiFi SSID: ")
        password = input("Enter WiFi Password: ")
    else:
        print("Need COM port, SSID and password")
        sys.exit(1)
else:
    port = sys.argv[1]
    ssid = sys.argv[2]
    password = sys.argv[3]

print(f"\nPort: {port}")
print(f"SSID: {ssid}")
print(f"\nOpening serial port and sending command...\n")

try:
    ser = serial.Serial(port, 115200, timeout=3)
    time.sleep(0.5)

    # Send the wifi command (multiple times to ensure it gets through)
    cmd = f"WIFI:{ssid},{password}\n"
    print(f"Sending: {cmd.strip()}")

    # Send 3 times with pauses
    for i in range(3):
        ser.write(cmd.encode())
        ser.flush()
        print(f"  Attempt {i+1} sent")
        time.sleep(1.5)

    # Read all response
    time.sleep(2)
    response = b""
    while True:
        try:
            chunk = ser.read(ser.in_waiting or 1)
            if not chunk:
                break
            response += chunk
            time.sleep(0.2)
        except:
            break

    if response:
        try:
            text = response.decode('utf-8', errors='replace')
            # Print last part of response (where the reboot message will be)
            print("\nDevice response:")
            for line in text.split('\n'):
                if line.strip():
                    print(f"  {line.strip()}")
        except:
            print(f"\nRaw response ({len(response)} bytes)")
    else:
        print("(no response - device may be booting)")

    ser.close()

    print("\n========== DONE ==========")
    print("If successful, the device will reboot and connect to WiFi.")
    print("Open ESP RainMaker App on your phone to see the device.")

except serial.SerialException as e:
    print(f"Error: {e}")
    print(f"Make sure {port} is correct and nothing else is using it.")
    sys.exit(1)
except Exception as e:
    print(f"Error: {e}")
    sys.exit(1)
