import serial, time

ser = serial.Serial('COM3', 115200, timeout=2)

# 先等芯片稳定启动并连上WiFi
print("等待芯片启动并连网...")
time.sleep(10)

# 读取启动日志，确认在线
data = ser.read(ser.in_waiting or 512)
text = data.decode('utf-8', errors='replace')
for line in text.split('\n'):
    if 'MQTT Connected' in line or 'UART listener' in line or 'Device ready' in line:
        print("  [芯片状态]", line.strip())

print("\n开始发送模拟命令...")
time.sleep(1)

# 1. 用户1指纹
print(">> 指纹识别 用户1")
ser.write(b'STM_FP:1\r\n')
time.sleep(2)

# 2. 三次出水
for i in range(3):
    print(f">> 出水完成 第{i+1}次 200ml")
    ser.write(b'STM_DONE:200\r\n')
    time.sleep(3)

print("\n从日志中提取关键信息：")
data = ser.read(ser.in_waiting or 512)
if data:
    text = data.decode('utf-8', errors='replace')
    for line in text.split('\n'):
        if any(kw in line for kw in ['Dispenser:', 'Fingerprint', 'Daily:', 'Next drink', 'Display:', 'Voice:']):
            print("  ", line.strip())

ser.close()
print("\n=== 模拟完成 ===")
