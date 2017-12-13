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
        #pygame.mixer.pre_init(48000, -16, 2, 2048)
        pygame.mixer.init()
        pygame.init()
        pygame.font.init()
        self.display = pygame.display.set_caption('Music Player')
        self.blue = (0, 100, 205)
        self.size = [640,480]
        self.screen = pygame.display.set_mode(self.size)
        self.clock = pygame.time.Clock()
        self.myfont = pygame.font.SysFont("freesandbold", 50)
        self.label = self.myfont.render("SoundCrystal", True, (0,255,0))

    def filesfolder(self):
        files = []
        path = 'C:/Users/apayano/Documents/GitHub/PoE-hI5-JuCube/Music/'
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
sound = musicplayer.picksong()
arduino = serial.Serial('COM11', 9600, timeout=.1)

def events(player, songs):
    i = 0
    loaded = False
    path = 'C:/Users/apayano/Documents/GitHub/PoE-hI5-JuCube/Music/'

    while True:
        data = arduino.readline()[:-2]
        END_MUSIC_EVENT = pygame.USEREVENT + 0
        pygame.mixer.music.set_endevent(END_MUSIC_EVENT)
        if data == b'1':
            i -= 1
            if i < 0:
                i = 15
            print(songs[i])
            song = songs[i]
            songlen = len(songs[i])
            songname = song[3:songlen-4]
            player.label = player.myfont.render(songname, True, (0,255,0))
            print("Back")
        if data == b'4':
            i += 1
            print(i)
            if i > 15:
                i = 0
            print(songs[i])
            song = songs[i]
            songlen = len(songs[i])
            songname = song[3:songlen-4]
            player.label = player.myfont.render(songname, True, (0,255,0))
            print("Forward")
        if data == b'2':
            print("Loading " + songs[i])
            player.label = player.myfont.render("Loading", True, (0,255,0))
            pygame.mixer.init(44100)
            pygame.mixer.music.load((path + songs[i]))
            loaded = True
        if data == b'01':
            if loaded == False:
                print('Load a song!')
            else:
                if pygame.mixer.music.get_busy() == False:
                    pygame.mixer.music.play()
                    pygame.mixer.music.set_volume(1)
                    player.label = player.myfont.render("Playing", True, (0,255,0))
                else:
                    pygame.mixer.music.unpause()
                print('Playing')
        if data == b'02':
            if loaded == False:
                print('Load a song!')
                player.label = player.myfont.render("Load a song!", True, (0,255,0))
            else:
                pygame.mixer.music.pause()
                player.label = player.myfont.render("Paused", True, (0,255,0))
                print('Paused')
                if pygame.mixer.music.get_busy() == False:
                    arduino.write(b'3')
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    sys.exit()
            if event.type == END_MUSIC_EVENT and event.code == 0:
                arduino.write(b'3')
        player.screen.fill(player.blue)
        pygame.display.update()
        player.screen.blit(player.label, (320 - player.label.get_width() // 2, 240 - player.label.get_height() // 2))
        pygame.display.flip()

musicplayer = Music_Player()
files = musicplayer.filesfolder()
events(musicplayer,songs)
