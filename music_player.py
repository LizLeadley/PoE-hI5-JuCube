import pygame
import multiprocessing
import os
import sys
import random
import threading
import time
import serial

class Music_Player():

    def __init__(self):
        pygame.init()
        pygame.mixer.init()
        self.display = pygame.display.set_caption('Music Player')
        self.blue = (0, 0, 255)
        self.size = [640,480]
        self.screen = pygame.display.set_mode(self.size)
        self.clock = pygame.time.Clock()

    def filesfolder(self):
        files = []
        path = 'C:/Users/apayano/Music/Music'
        directory = os.listdir(path)
        for filename in directory:
            if filename.endswith(".mp3"):
                files.append(filename)
        files.sort()
        return files

    def picksong(self):
        files = self.filesfolder()
        song = random.choice(files)
        print(song)
        return song


musicplayer = Music_Player()
sound = musicplayer.picksong()
arduino = serial.Serial('COM12', 9600, timeout=.1)

def events(player, sound):
    while True:
        data = arduino.readline()[:-2]
        if data == b'11':
            pygame.mixer.music.load(('C:/Users/apayano/Music/Music/' + sound))
            pygame.mixer.music.play()
            print('playing')
        if data == b'21':
            sound = musicplayer.picksong()
            pygame.mixer.music.stop()
            pygame.mixer.music.load(('C:/Users/apayano/Music/Music/' + sound))
            pygame.mixer.music.play()
            print('playing')
        if data == b'12':
            pygame.mixer.music.stop()
            print('stop')
        if data == b'22':
            pygame.mixer.music.stop()
            print('stop')
        for event in pygame.event.get():
                if event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    sys.exit()
        player.screen.fill(player.blue)
        pygame.display.update()

musicplayer = Music_Player()
files = musicplayer.filesfolder()

print('press p - play sound')
print('press c - change song')
events(musicplayer,sound)
