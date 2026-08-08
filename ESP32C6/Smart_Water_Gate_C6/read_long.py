import serial, time, sys
ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 115200
ser.timeout = 1
ser.dtr = False
ser.rts = False
ser.open()
print("读取中，请发数据...")
time.sleep(1)
start = time.time()
while time.time() - start < 15:
    try:
        data = ser.read(ser.in_waiting or 0.5)
        if data:
            text = data.decode('utf-8', errors='replace')
            if 'UART1' in text or 'STM_FP' in text or 'STM_DONE' in text:
                print(">>> 收到:", text.strip())
            sys.stdout.write(text)
            sys.stdout.flush()
    except: pass
ser.close()
print("\n读取结束")
