import serial
import os

path = 'C:\Users\mlao\Downloads\APril18.mp3'
arduino = serial.Serial('COM5', 9600, timeout=.1)
while True:
	data = arduino.readline()[:-2]
	if data == '1':
		os.startfile(path)
	if data == '2':
		os.system('TASKKILL /F /IM wmplayer.exe')
		