import serial
import os

path1 = 'C:/Users/apayano/Music/Music/Breaking_Free_Remix.mp3'
path2 = 'C:/Users/apayano/Music/Music/Chance The Rapper- Acid Rap/04. Juice.mp3'
arduino = serial.Serial('COM12', 9600, timeout=.1)
while True:
	data = arduino.readline()[:-2]
	if data == b'11':
		print('yes')
		os.system('TASKKILL /F /IM wmplayer.exe')
		os.startfile(path1)
		print('playing')
	if data == b'21':
		os.system('TASKKILL /F /IM wmplayer.exe')
		os.startfile(path2)
		print('playing')
	if data == b'12':
		os.system('TASKKILL /F /IM wmplayer.exe')
		print('stop')
	if data == b'22':
		os.system('TASKKILL /F /IM wmplayer.exe')
		print('stop')
