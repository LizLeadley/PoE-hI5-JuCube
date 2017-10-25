import serial
import os

path1 = 'C:\Users\mlao\Desktop\POE\JukeboxHero.mp3'
path2 = 'C:\Users\mlao\Desktop\POE\ShapeOfYou.mp3'
arduino = serial.Serial('COM5', 9600, timeout=.1)
while True:
	data = arduino.readline()[:-2]
	if data == '11':
		print data
		os.system('TASKKILL /F /IM wmplayer.exe')
		os.startfile(path1)
	if data == '21':
		os.system('TASKKILL /F /IM wmplayer.exe')
		os.startfile(path2)
	if data == '12':
		os.system('TASKKILL /F /IM wmplayer.exe')
	if data == '22':
		os.system('TASKKILL /F /IM wmplayer.exe')
