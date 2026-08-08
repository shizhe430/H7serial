import serial, time

# 打开串口时不触发DTR/RTS复位
ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 115200
ser.timeout = 2
ser.dtr = False
ser.rts = False
ser.open()

time.sleep(3)

print(">> 指纹识别 用户1")
ser.write(b'STM_FP:1\r\n')
time.sleep(2)

for i in range(3):
    print(f">> 出水完成 第{i+1}次 200ml")
    ser.write(b'STM_DONE:200\r\n')
    time.sleep(2)

# 看日志
data = ser.read(ser.in_waiting or 256)
if data:
    text = data.decode('utf-8', errors='replace')
    for line in text.split('\n'):
        if any(kw in line for kw in ['Dispenser:', 'Fingerprint', 'Daily:', 'Next drink', 'Display:', 'Voice:']):
            print("  ", line.strip())

ser.close()
print("=== 完成（芯片未复位，WiFi不掉线）===")
