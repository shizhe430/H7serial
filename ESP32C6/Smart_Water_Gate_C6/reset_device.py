import serial, time
ser = serial.Serial('COM3', 115200, timeout=0.5)
ser.setDTR(False)
ser.setRTS(False)
time.sleep(0.5)
# Toggle DTR/RTS for reset
ser.setDTR(True)
time.sleep(0.1)
ser.setDTR(False)
time.sleep(0.5)
ser.close()
print("Device reset complete")
