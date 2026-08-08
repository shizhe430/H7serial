import serial, time, sys
ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 115200
ser.timeout = 2
ser.dtr = False
ser.rts = False
ser.open()
time.sleep(1)
# 只读5秒，不发数据
start = time.time()
while time.time() - start < 5:
    try:
        data = ser.read(ser.in_waiting or 0.5)
        if data:
            sys.stdout.write(data.decode('utf-8', errors='replace'))
            sys.stdout.flush()
    except: pass
ser.close()
