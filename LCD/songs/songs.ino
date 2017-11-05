/* YourDuino.com Example Software Sketch
 16 character 2 line I2C Display
 Backpack Interface labelled "A0 A1 A2" at lower right.
 ..and
 Backpack Interface labelled "YwRobot Arduino LCM1602 IIC V1"
 MOST use address 0x27, a FEW use 0x3F
 terry@yourduino.com */

// Works for only top line

/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x3E, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

/*-----( Declare Variables )-----*/
const int buttonPin = 2;
int buttonState = 0; 
int lastButtonState = 0;
unsigned long previousMillis = 0;        
unsigned long currentMillis = 0;
unsigned long debouncingMillis = 0;
unsigned long lcdMillis = 0;
int buttonCounter = 1;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  // Used to type in characters

  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
  pinMode(buttonPin, INPUT);
// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  
  lcd.setCursor(5,0); //Start at character 4 on line 0
  lcd.print("JuCube");
  delay(1000);
  

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  buttonState = digitalRead(buttonPin);
  // start timer
  currentMillis = millis();
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (buttonState != lastButtonState) {
      // if the state has changed (button has been pressed), increment the counter
      if (buttonState == HIGH) {
        buttonCounter++;
        Serial.println(buttonCounter);
      }
    //reset the debounce timer
    debouncingMillis = currentMillis;
    }
  }
  lastButtonState = buttonState;
  if (buttonCounter == 1) {
    //first song
    if (currentMillis - lcdMillis >= 500) {
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Eye of the Tiger");
      lcdMillis = currentMillis;
    }
  }
  if (buttonCounter == 2) {
    //second song
    if (currentMillis - lcdMillis >= 500) {
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("I Love");
      lcd.setCursor(0,1);
      lcd.print("Rock and Roll");
      lcdMillis = currentMillis;
    }
  }
  
  if (buttonCounter == 3) {
    if (currentMillis - lcdMillis >= 500) {
      //third song
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Jukebox Hero");
      lcdMillis = currentMillis;
    }
  }
  
  if (buttonCounter == 4) {
    if (currentMillis - lcdMillis >= 500) {
        //fourth song
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Shape of You");
      lcdMillis = currentMillis;
    }
  }
  
  if (buttonCounter == 5) {
    if (currentMillis - lcdMillis >= 500) {
      //fifth song
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("We Will Rock You");
      lcdMillis = currentMillis;
    }
  }
  
  if (buttonCounter >= 6) {
    buttonCounter = 1;
  }
}/* --(end main loop )-- */


/* ( THE END ) */
