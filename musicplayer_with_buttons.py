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
        path = 'C:/Users/apayano/Documents/GitHub/PoE-hI5-JuCube/Music player/Music/'
        directory = os.listdir(path)
        for filename in directory:
            if filename.endswith(".mp3"):
                files.append(filename)
        files.sort()
        return files

    def picksong(self):
        files = self.filesfolder()
        song = random.choice(files)
        return song


musicplayer = Music_Player()
songs = musicplayer.filesfolder()
print(songs)
sound = musicplayer.picksong()
arduino = serial.Serial('COM12', 9600, timeout=.1)

def events(player, songs):
    i = 1
    path = 'C:/Users/apayano/Documents/GitHub/PoE-hI5-JuCube/Music player/Music/'
    while True:
        data = arduino.readline()[:-2]
        if data == b'1':
            i -= 1
            if i < 0:
                i = 4
            print('Playing ' + songs[i])
            pygame.mixer.music.load((path + songs[i]))
        if data == b'2':
            i += 1
            if i > 4:
                i = 0
            print('Playing ' + songs[i])
            pygame.mixer.music.load((path + songs[i]))
        if data == b'01':
            pygame.mixer.music.stop()
            pygame.mixer.music.play()
        if data == b'02'
            pygame.mixer.music.stop()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    sys.exit()
        player.screen.fill(player.blue)
        pygame.display.update()

musicplayer = Music_Player()
files = musicplayer.filesfolder()

print('press p - play sound')
print('press c - change song')
events(musicplayer,songs)
