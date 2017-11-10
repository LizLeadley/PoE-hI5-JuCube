
/* 
 *  Buttons and LED Strip
 */
#include <LiquidCrystal_I2C.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Servo.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

Servo needle_tilt_servo; // creates servo object

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

int lcdCounter = 0;


// will store last time LED was updated    
unsigned long lcdMillis = 0;   
unsigned long currentMillis = 0;
unsigned long debouncingMillis = 0;

//LED STRIP STUFF
int prevVal = 0; //previous value for cleaning high frequencies
int signalRead = A0; //input pin from transistor signal
uint32_t hotPink = strip.Color(255, 25, 128); 
double pink_r_g = 25.0/255.0;
double pink_r_b = 128.0/255.0;
uint32_t blue = strip.Color(35, 73, 224);
double blue_b_r = 35.0/224.0; //blue ratio for blue to red
double blue_b_g = 73/224.0;  //blue ratio for blue to green
double green_g_r = 66.0/244.0;
double green_g_b = 197.0/244.0;
double orange_o_b = 10.0/255.0;
double orange_o_g = 60.0/255.0; 



int pos = 0; // position, for servo sweep iterator
float servo_step = 1; // step size for servo sweep
unsigned int servo_delay = 20;
unsigned int prev_millis = 0;
unsigned int cur_millis = 0;

// runs all tests if true
bool debug = false;

// music paused boolean
bool paused = false;

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
    pinMode(button1,INPUT);
    pinMode(button2,INPUT);
    pinMode(button3,INPUT);

    needle_tilt_servo.attach(9); // attaches servo to pin 5

    needle_tilt_servo.write(90);
    
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

  // lift and lower record needle
  //new_song_tilt(needle_tilt_servo, servo_delay);

  if (debug) {
    servo_test();  
  }
}

// put your main code here, to run repeatedly
void loop(){

//return;
  
    //new_song_tilt(needle_tilt_servo, servo_delay);
    // read the pushbutton input pin:
    button1State = digitalRead(button1);
    button2State = digitalRead(button2);
    button3State = digitalRead(button3);
    // start timer
    currentMillis = millis();
  //LED STRIP CODE:
      double val = analogRead(signalRead);
      if ( val > 15 ) {
      val = 700;
      }
      double newVal = map(val, 0, 1023, 0, 200);
      double giveVal = round(.5*(newVal+prevVal));
      uint32_t pinkBeat = strip.Color(giveVal,giveVal*(pink_r_g),giveVal*(pink_r_b)); //uses relative rgb values for pink

      uint32_t blueBeat = strip.Color(giveVal*(blue_b_r), giveVal*(blue_b_g), giveVal); //uses relative rgb values for blue
<<<<<<< HEAD
      uint32_t greenBeat = strip.Color(giveVal*(green_g_r), giveVal, giveVal*(green_g_b)); //relative color for green
      uint32_t orangeBeat = strip.Color(giveVal, giveVal*(orange_o_g), giveVal*(orange_o_b)); //relative color for orange
      //setColor(orangeBeat); //set initial color
      setTwoColor(pinkBeat, blueBeat);
=======

      setColor(blueBeat); //can be either pinkBeat or blueBeat
            return;
>>>>>>> 371c36b5db1b96e2a177f66d1fde14b22e529928
      prevVal = newVal;
return;


      cur_millis = millis();
   
    //  delay(500);


//return;

 
  //button1 debounce (choose song)
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
  //button2 debounce (load song)
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button2State != lastButton2State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button2State == HIGH) {
        Serial.println("2");
        button3Counter = 0;
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
    }
  }
  //button3 debounce (play/pause)
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button3State != lastButton3State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button3State == HIGH) {
        button3Counter++;
        Serial.print("0");
        Serial.println(button3Counter);
        if (paused) {
          paused = false;
        }
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
      if (button3Counter == 2) {
        button3Counter = 0;
      }
    }
  }
  lastButton1State = button1State;
  lastButton2State = button2State;
  lastButton3State = button3State;


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
      lcd.setCursor(5,0); //Start at character 4 on line 0
      lcd.print("I Love");
      lcd.setCursor(2,1);
      lcd.print("Rock N Roll");
      lcdMillis = currentMillis;
    }
  }
  
  if (lcdCounter == 3) {
    if (currentMillis - lcdMillis >= 500) {
      //third song
      lcd.clear();
      lcd.setCursor(2,0); //Start at character 4 on line 0
      lcd.print("Jukebox Hero");
      lcdMillis = currentMillis;
    }
  }
  
  if (lcdCounter == 4) {
    if (currentMillis - lcdMillis >= 500) {
        //fourth song
      lcd.clear();
      lcd.setCursor(2,0); //Start at character 4 on line 0
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
//  // tilt needle in new song starts playing
  if (paused) {
      //new_song_tilt(needle_tilt_servo, servo_delay);
  }

}

void setColor(uint32_t color) {
  uint16_t i;
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

<<<<<<< HEAD
void setTwoColor(uint32_t color1, uint32_t color2) {
  uint16_t i;
  for(i=0; i<strip.numPixels()/2; i++) {
    strip.setPixelColor(i, color1);
  }
  strip.show();
  for(i=strip.numPixels()/2; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color2);
  }
  strip.show();
}

int new_song_tilt(Servo needle_tilt_servo, unsigned int servo_delay) {
=======
void new_song_tilt(Servo needle_tilt_servo, unsigned int servo_delay) {
>>>>>>> 371c36b5db1b96e2a177f66d1fde14b22e529928
  tilt_up(needle_tilt_servo, servo_delay);
  tilt_down(needle_tilt_servo, servo_delay);
}

void tilt_up(Servo needle_tilt_servo, unsigned int servo_delay) {
  unsigned int prev_millis = 0;
  boolean servo_step_move_done = false;
  boolean running_correctly = true;
  boolean needle_move_up_done = false;
  int pos = 0;
  while(!needle_move_up_done){
    unsigned int cur_millis = millis(); 
    // if it has, check that the delay has passed
    if (cur_millis - prev_millis >= servo_delay) {
      // update prev_millis
      prev_millis += servo_delay;
      if (++pos > 90) {
        needle_move_up_done = true;
      }
      else {
        needle_tilt_servo.write(pos);
        servo_step_move_done = true;
        pos ++;
      }
    }
}
}

void tilt_down(Servo needle_tilt_servo, unsigned int servo_delay) {
  unsigned int prev_millis = 0;
  boolean servo_step_move_done = false;
  boolean needle_move_down_done = false;
  int pos = 90;
  while(!needle_move_down_done){
    unsigned int cur_millis = millis();
    // if it has, check that the delay has passed
    if (cur_millis - prev_millis >= servo_delay) {
      // update prev_millis
      prev_millis += servo_delay;
      if (--pos < 0) {
        needle_move_down_done = true;
      }
      else {
        needle_tilt_servo.write(pos);
        servo_step_move_done = true;
        pos --;
      }
    }
}
}

void servo_test(){
  // test servo
  needle_tilt_servo.write(90);
  lcd.clear();
  lcd.print("Sevo Test");
}


