/*
    Buttons and LED Strip
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h> // library for serial communication

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

const byte button1 = 2; //green -- back
const byte button2 = 3; //blue -- select
const byte button3 = 4; // -- play
const byte button4 = 5; // -- forward
const byte motorPin = 7;

LiquidCrystal_I2C lcd(0x3E, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int button3Counter = 0;
int button1State; //Skip back
int button2State; // Select
int button3State; // Play
int button4State; // Skip forward
int lastButton1State = LOW;
int lastButton2State = LOW;
int lastButton3State = LOW;
int lastButton4State = LOW;

unsigned long currentMillis = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 10;
unsigned long currentScroll = 0;
unsigned long scrollDelay = 150;


const char* Songs[]= {"Jukebox Hero", "Shape Of You", "Eye Of The Tiger", "I Love Rock N' Roll", "We Will Rock You", "Don't Stop Believing",
"Stereo Hearts", "Feel It Still", "Bohemian Rhapsody", "Sail", "Freaks", "Paper Planes", "Jump On It", "Heads Will Roll", 
"Take A Walk", "Beat It"};

int lcdCounter = 0;


// will store last time LED was updated
unsigned long lcdMillis = 0;
unsigned long debouncingMillis = 0;

//LED STRIP STUFF
int prevVal = 0; //previous value for cleaning high frequencies
int signalRead = A0; //input pin from transistor signal
uint32_t hotPink = strip.Color(255, 25, 128);
double pink_r_g = 25.0 / 255.0;
double pink_r_b = 128.0 / 255.0;
uint32_t blue = strip.Color(35, 73, 224);
double blue_b_r = 35.0 / 224.0; //blue ratio for blue to red
double blue_b_g = 73 / 224.0; //blue ratio for blue to green
double green_g_r = 66.0 / 244.0;
double green_g_b = 197.0 / 244.0;
double orange_o_b = 10.0 / 255.0;
double orange_o_g = 60.0 / 255.0;

/*
   Activation stuff variables
*/

//Marble activation button stuff
bool activated = 0;

//buttonA for Activation Button
const byte buttonA = 13;
bool buttonAState = LOW;
bool lastButtonAState = LOW;
bool buttonAPress = 0;
const int buttonDebounceLimit = 10;


// setup routine runs once when you press reset (only runs once)
void setup() {
  pinMode(signalRead, INPUT); //set the analog pin as an input
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(245); //set max brightness

  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(buttonA, INPUT);
  pinMode(motorPin, OUTPUT);

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
  delay(2000);
  lcd.clear();
  lcd.setCursor(1, 0); //Start somewhere on top line
  lcd.print("Insert marble");
  lcd.setCursor(4, 1); //hopfully starts on second line
  lcd.print("to begin");

  digitalWrite(motorPin,LOW);
  Serial.println("motor off");
}

// put your main code here, to run repeatedly
void loop() {

  /*  Check if activation button has been pressed
    If not, wait for button press
    If pressed, advance to the rest of the loop
  */
  if (activated == 0) {

    //    LCD display "Insert marble to begin"
    lcd.clear();
    lcd.setCursor(1, 0); //Start somewhere on top line
    lcd.print("Insert marble");
    lcd.setCursor(4, 1); //hopfully starts on second line
    lcd.print("to begin");

    //Wait in loop for activation button to be pressed
    while (activated == 0) {

      //check button with special debounce
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
        return;
      }
    }
  }
// End of activation = 0 statement and loop

  // read the pushbutton input pin:
  int read1 = digitalRead(button1);
  int read2 = digitalRead(button2);
  int read3 = digitalRead(button3);
  int read4 = digitalRead(button4);
  // start timer
  currentMillis = millis();
/*
 * Check all the buttons, with debounces
 */
  //button1 debounce (back)
      // start timer
      if (read1 != lastButton1State) {
        lastDebounceTime = millis();
      }
      if ((millis()- lastDebounceTime) > debounceDelay) {
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
      if ((millis()- lastDebounceTime) > debounceDelay) {
        if (read2 != button2State) {
          button2State = read2;
          if (button2State == HIGH) {
            Serial.println("2");
            button3Counter = 0;
            lcd.clear();
            lcd.setCursor(9,1);
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
      if ((millis()- lastDebounceTime) > debounceDelay) {
        if (read3 != button3State) {
          button3State = read3;
          if (button3State == HIGH) {
            button3Counter++;
            Serial.print("0");
            Serial.println(button3Counter);
          }
          if (button3Counter == 2) {
            button3Counter = 0;
          }
          if (button3Counter == 0) {
            lcd.clear();
            lcd.setCursor(10,1);
            lcd.print("Paused");
          }
          if (button3Counter == 1) {
            lcd.clear();
            lcd.setCursor(9,1);
            lcd.print("Playing");
          }
        }
      }
      lastButton3State = read3;

      if(Serial.available() > 0) {
        char number = Serial.read();
        if (number == '3') { 
          button3Counter = 0;
        }
      }
    //button4 debounce (forward)
      // start timer
      if (read4 != lastButton4State) {
        lastDebounceTime = millis();
      }
      if ((millis()- lastDebounceTime) > debounceDelay) {
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
    digitalWrite(motorPin,LOW);
    // Serial communication to record needle
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(0);              // sends 0 for song not playing
    Wire.endTransmission();    // stop transmitting
  }
  if (button3Counter == 1) {
    digitalWrite(motorPin,HIGH);
    // Serial communication to record needle
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(1);              // sends 1 for song playing
    Wire.endTransmission();    // stop transmitting
  }

/*
 * Set the LCD to display what song is looked at
 */
    //first song
  if (activated == 1){
    if (millis() - lcdMillis >= 500) {
      lcd.clear();
      String song = Songs[lcdCounter];
      if (song.length() > 15) {
        int firstSpace = song.indexOf(' ', 4);
        lcd.setCursor(0,0);
        lcd.print(song.substring(0, firstSpace));
        lcd.setCursor(0,1);
        lcd.print(song.substring(firstSpace+1));  
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

void setColor(uint32_t color) {
  uint16_t i;
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setTwoColor(uint32_t color1, uint32_t color2) {
  uint16_t i;
  for (i = 0; i < strip.numPixels() / 2; i++) {
    strip.setPixelColor(i, color1);
  }
  strip.show();
  for (i = strip.numPixels() / 2; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color2);
  }
  strip.show();
}


