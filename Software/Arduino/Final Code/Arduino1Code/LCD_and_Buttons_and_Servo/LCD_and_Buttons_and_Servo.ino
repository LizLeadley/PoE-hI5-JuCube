#include <LiquidCrystal_I2C.h> // library for LCD Screen
#include <Wire.h> // library for serial communication
#include <Servo.h> // library for servo

Servo needle_tilt_servo; //creates servo object

#include <Adafruit_NeoPixel.h> //library for NeoPixel
#ifdef __AVR__
#include <avr/power.h>
#endif

// Max and Min Servo Positions
const int min_pos = 35;
const int max_pos = 100;

// booleans for needle tilt
bool needle_go = false;
bool needle_moved = false;

// pins for music player buttons
const byte button1 = 2; //green -- back
const byte button2 = 3; //blue -- select
const byte button3 = 4; // -- play
const byte button4 = 5; // -- forward

//pin for dc motor
const byte motorPin = 7;

//initialization of LCD Screen
LiquidCrystal_I2C lcd(0x3E, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int button3Counter = 0; //counter for the play/pause
int button1State; //Skip back
int button2State; // Select
int button3State; // Play
int button4State; // Skip forward
int lastButton1State = LOW;
int lastButton2State = LOW;
int lastButton3State = LOW;
int lastButton4State = LOW;

//timers
unsigned long currentMillis = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 10;
unsigned long currentScroll = 0;
unsigned long scrollDelay = 150;

//Songlist for LCD and counter for LCD
const char* Songs[] = {"Jukebox Hero", "Shape Of You", "Eye Of The Tiger", "I Love Rock N' Roll", "We Will Rock You", "Don't Stop Believing",
                       "Stereo Hearts", "Feel It Still", "Bohemian Rhapsody", "Sail", "Freaks", "Paper Planes", "Jump On It", "Heads Will Roll",
                       "Take A Walk", "Beat It"
                      };
int lcdCounter = 0;


// will store last time LED was updated
unsigned long lcdMillis = 0;
unsigned long debouncingMillis = 0;

//Marble activation button stuff
bool activated = 0;
// const int activationWaitLimit = 20 * 1000; //number of seconds to wait before assuming activation failure and starting anyway
long initialMillis;

//buttonA for Activation Button
const byte buttonA = 13;
bool buttonAState = LOW;
bool lastButtonAState = LOW;
bool buttonAPress = 0;
const int buttonDebounceLimit = 10;

// setup routine runs once when you press reset (only runs once)
void setup() {

  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(buttonA, INPUT);
  pinMode(motorPin, OUTPUT);

  // initializes and positions servo (record needle)
  needle_tilt_servo.attach(6);
  needle_tilt_servo.write(max_pos);

  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight
  // ------- Quick 3 blinks of backlight  -------------
  for (int i = 0; i < 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on

  //-------- Write characters on the display ------------------
  // NOTE: Cursor Position: (CHAR, LINE) start at 0
  lcd.setCursor(2, 0); //Start at character 4 on line 0
  lcd.print("SoundCrystal");
  delay(5000);

  digitalWrite(motorPin, LOW);
  Serial.println("motor off");



}


// put your main code here, to run repeatedly
void loop() {

  /*     ------ Check all the buttons, with debounces ------   */
  // read the pushbutton input pin:
  int read1 = digitalRead(button1);
  int read2 = digitalRead(button2);
  int read3 = digitalRead(button3);
  int read4 = digitalRead(button4);
  // start timer
  currentMillis = millis();

  //button1 debounce (back)
  // start timer
  if (read1 != lastButton1State) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (read1 != button1State) {
      button1State = read1;
      if (button1State == HIGH) {
        lcdCounter--;
        Serial.println("1");
        if (lcdCounter < 0) {
          lcdCounter = 15;
        }
      }
    }
  }
  lastButton1State = read1;
  //button2 debounce (select)
  // start timer
  if (read2 != lastButton2State) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (read2 != button2State) {
      button2State = read2;
      if (button2State == HIGH) {
        Serial.println("2");
        button3Counter = 0;
        lcd.clear();
        lcd.setCursor(9, 1);
        lcd.print("Loading");
      }
    }
  }
  lastButton2State = read2;
  //button3 debounce (play/pause song)
  // start timer
  if (read3 != lastButton3State) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (read3 != button3State) {
      button3State = read3;
      if (button3State == HIGH) {
        button3Counter++; //increase buttoncounter to figure out whether or not its a play or pause press
        Serial.print("0");
        Serial.println(button3Counter);
      }
      if (button3Counter == 2) { //reset counter
        button3Counter = 0;
      }
      if (button3Counter == 0) { //pause
        lcd.clear();
        lcd.setCursor(10, 1);
        lcd.print("Paused");
      }
      if (button3Counter == 1) { //play
        lcd.clear();
        lcd.setCursor(9, 1);
        lcd.print("Playing");
      }
    }
  }
  lastButton3State = read3;

  if (Serial.available() > 0) { //set record spinner and needle to off state when song ends
    char number = Serial.read();
    if (number == '3') {
      button3Counter = 0;
      activated = 0;
    }
  }
  /* ------  Activation button ------ */
   if (activated == 0) {
    lcd.clear();
    lcd.setCursor(1, 0); //Start somewhere on top line
    lcd.print("Insert marble");
    lcd.setCursor(4, 1); //hopfully starts on second line
    lcd.print("to begin");
    digitalWrite(motorPin, LOW);
    needle_go = false;
    needle_moved = false;
    needle_tilt_servo.write(min_pos);
   }
   while (activated == 0) {
  //check button with small debounce
    buttonAState = digitalRead(buttonA);
    // start timer
    currentMillis = millis();
    if (currentMillis - debouncingMillis >= buttonDebounceLimit) {
      //check to see if button has been pressed 
      if (buttonAState != lastButtonAState) {
        // if the state has changed (button has been pressed), increment the counter
        if (buttonAState == HIGH) {
          buttonAPress = 1;
        }
      }
      //reset the debounce timer
      debouncingMillis = currentMillis;
    }
  
    if (buttonAPress == 1) {
      buttonAPress = 0;
      activated = 1;
      Serial.println("activated");
    }
  }
  //button4 debounce (forward)
  // start timer
  if (read4 != lastButton4State) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (read4 != button4State) {
      button4State = read4;
      if (button4State == HIGH) {
        lcdCounter++;
        Serial.println("4");
        if (lcdCounter > 15) {
          lcdCounter = 0;
        }
      }
    }
  }
  lastButton4State = read4;

  if (button3Counter == 0) {
    digitalWrite(motorPin, LOW);
    needle_go = false;
    needle_moved = false;
    needle_tilt_servo.write(min_pos);
  }
  if (button3Counter == 1) {
    digitalWrite(motorPin, HIGH);
    needle_go = true;
  }

  if (needle_go && !needle_moved) {
    needle_tilt_servo.write(max_pos);
    needle_moved = true;
    needle_go = false;
  }

  /* ------ Set the LCD to display what song is looked at ------ */
  //first song
  if (activated == 1) {
    if (millis() - lcdMillis >= 500) {
      lcd.clear();
      String song = Songs[lcdCounter];
      if (song.length() > 15) {
        int firstSpace = song.indexOf(' ', 4);
        lcd.setCursor(0, 0);
        lcd.print(song.substring(0, firstSpace));
        lcd.setCursor(0, 1);
        lcd.print(song.substring(firstSpace + 1));
        lcdMillis = currentMillis;
      }
      else {
        lcd.setCursor(0, 0);//Start at character 4 on line 0
        lcd.print(Songs[lcdCounter]);
        lcdMillis = currentMillis;
      }
    }
  }


}
//End of main loop
