import serial
arduino = serial.Serial('COM5', 9600, timeout=.1)
while True:
	data = arduino.readline()[:-2]
	if data:
		print data