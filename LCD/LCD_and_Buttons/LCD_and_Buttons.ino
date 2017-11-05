/* 
 *  Buttons and LED Strip
 */
#include <LiquidCrystal_I2C.h>

const byte button1 = 2; //green
const byte button2 = 3; //blue
const byte button3 = 4;


LiquidCrystal_I2C lcd(0x3E, 2, 1, 0, 4, 5, 6, 7, 3,POSITIVE); 

int button3Counter = 0;
int button1State = 0;
int button2State = 0;
int button3State = 0;
int lastButton1State = 0;
int lastButton2State = 0;
int lastButton3State = 0;

unsigned long lcdMillis = 0;
int lcdCounter = 1;


// will store last time LED was updated
unsigned long previousMillis = 0;        
unsigned long currentMillis = 0;
unsigned long debouncingMillis = 0;


// setup routine runs once when you press reset (only runs once)
void setup() {
    Serial.begin(9600);
    pinMode(button1,INPUT);
    pinMode(button2,INPUT);
    pinMode(button3, INPUT);  

    lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
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
}

// put your main code here, to run repeatedly
void loop(){
    // read the pushbutton input pin:
    button1State = digitalRead(button1);
    button2State = digitalRead(button2);
    button3State = digitalRead(button3);
    // start timer
    currentMillis = millis();

  //button1 debounce
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button1State != lastButton1State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button1State == HIGH) {
        // increment counter to change the LED mode
        lcdCounter--;
        if (lcdCounter <= 0) {
          lcdCounter = 5;
        }
        Serial.println("1");
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
    }
  }
  //button2 debounce
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button2State != lastButton2State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button2State == HIGH) {
        // increment counter to change the LED mode
        lcdCounter++;
        if (lcdCounter >= 6) {
          lcdCounter = 1;
        }
        Serial.println("2");
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
    }
  }
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button3State != lastButton3State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button3State == HIGH) {
        // increment counter to change the LED mode
        button3Counter++;
        Serial.print("0");
        Serial.print(button3Counter);
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
      if (button3Counter == 2){
        button3Counter = 0;
    }
  }
  lastButton1State = button1State;
  lastButton2State = button2State;

  if (lcdCounter == 1) {
    //first song
    if (currentMillis - lcdMillis >= 500) {
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Eye of the Tiger");
      lcdMillis = currentMillis;
    }
  }
  if (lcdCounter == 2) {
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
  
  if (lcdCounter == 3) {
    if (currentMillis - lcdMillis >= 500) {
      //third song
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Jukebox Hero");
      lcdMillis = currentMillis;
    }
  }
  
  if (lcdCounter == 4) {
    if (currentMillis - lcdMillis >= 500) {
        //fourth song
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("Shape of You");
      lcdMillis = currentMillis;
    }
  }
  
  if (lcdCounter == 5) {
    if (currentMillis - lcdMillis >= 500) {
      //fifth song
      lcd.clear();
      lcd.setCursor(0,0); //Start at character 4 on line 0
      lcd.print("We Will Rock You");
      lcdMillis = currentMillis;
    }
  }
}
