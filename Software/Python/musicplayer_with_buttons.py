import pygame
import os
import time
import serial

class Music_Player():

    def __init__(self):
        pygame.mixer.init() #initialize pygame mixer
        pygame.init() # initialize pygame
        pygame.font.init() #initialize pygame font
        self.display = pygame.display.set_caption('Music Player') #set display
        self.blue = (0, 100, 205) #set color
        self.size = [640,480] #set size of display
        self.screen = pygame.display.set_mode(self.size) #open screen
        self.clock = pygame.time.Clock() #start timer
        self.myfont = pygame.font.SysFont("freesandbold", 50) #set font
        self.label = self.myfont.render("SoundCrystal", True, (0,255,0)) #write on screen

    def filesfolder(self):
        files = []
        path = 'C:/Users/apayano/Documents/GitHub/PoE-hI5-JuCube/Music/' #change path to wherever music files can be found
        directory = os.listdir(path)
        for filename in directory:
            if filename.endswith(".mp3"): #if wav files, change to ".wav"
                files.append(filename)
        files.sort()
        return files

arduino = serial.Serial('COM11', 9600, timeout=.1) #initialize Arduino serial port

def events(player, songs):
    i = 0
    loaded = False
    song = songs[i]
    songlen = len(songs[i])
    songname = song[3:songlen-4]


    while True:
        data = arduino.readline()[:-2] #start reading from Arduino serial
        #conditions if buttons are pressed
        if data == b'1': #serial outputs "1" meaning back button is pressed
            i -= 1
            if i < 0:
                i = 15
            print(songs[i])
            player.label = player.myfont.render(songname, True, (0,255,0))
            print("Back")
        if data == b'4': #serial outputs "4" meaning forward button is pressed
            i += 1
            print(i)
            if i > 15:
                i = 0
            print(songs[i])
            player.label = player.myfont.render(songname, True, (0,255,0))
            print("Forward")
        if data == b'2': #serial outputs "2" meaning load button is pressed
            print("Loading " + songs[i])
            player.label = player.myfont.render("Loading", True, (0,255,0))
            pygame.mixer.init(44100) #set frequency of mp3 file
            pygame.mixer.music.load((path + songs[i])) #load mp3 file
            loaded = True
        if data == b'01': #serial outputs "01" meaning play/pause button is pressed - should play song
            if loaded == False:
                print('Load a song!') #make sure song is loaded before clicking play/pause - will close out
            else:
                if pygame.mixer.music.get_busy() == False:
                    pygame.mixer.music.play()
                    player.label = player.myfont.render("Playing", True, (0,255,0))
                else:
                    pygame.mixer.music.unpause()
                print('Playing')
        if data == b'02': #serial outputs "02" meaning play/pause is pressed - should pause song
            if loaded == False:
                print('Load a song!')
                player.label = player.myfont.render("Load a song!", True, (0,255,0)) #make sure song is loaded
            else:
                pygame.mixer.music.pause()
                player.label = player.myfont.render("Paused", True, (0,255,0))
                print('Paused')
                if pygame.mixer.music.get_busy() == False: #send data to arduino to stop motor and servo
                    arduino.write(b'3')
        for event in pygame.event.get():
            if event.type == pygame.QUIT: #close window
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE: #hit escape to also close window
                    pygame.quit()
                    sys.exit()
            if event.type == END_MUSIC_EVENT and event.code == 0:  #send data to arduino to stop motor and servo
                arduino.write(b'3')
        #update screen to say what state/button has been pressed
        player.screen.fill(player.blue)
        pygame.display.update()
        player.screen.blit(player.label, (320 - player.label.get_width() // 2, 240 - player.label.get_height() // 2))
        pygame.display.flip()

musicplayer = Music_Player()
songs = musicplayer.filesfolder()
events(musicplayer,songs) #run code
