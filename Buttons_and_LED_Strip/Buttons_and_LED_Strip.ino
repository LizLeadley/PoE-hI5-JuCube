/* 
 *  Buttons and LED Strip
 */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
const byte button1 = 2; //green
const byte button2 = 3; //blue

const int jukeBoxHero = 177;
const int shapeOfYou = 96;
const int offColorDelay = 50; //ms

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

int button1Counter = 0;
int button2Counter = 0;
int button1State = 0;
int button2State = 0;
int lastButton1State = 0;
int lastButton2State = 0;

double beatTime = 0;

// will store last time LED was updated
unsigned long previousMillis = 0;        
unsigned long currentMillis = 0;
unsigned long debouncingMillis = 0;

//Colors:
uint32_t stripOff = strip.Color(0, 0, 0); 

uint32_t turqoise = strip.Color(97, 232, 214); 
uint32_t magenta = strip.Color(235, 29, 29); 

uint32_t brightGreen = strip.Color(135, 255, 7); 
uint32_t yellow = strip.Color(255, 230, 12); 

uint32_t darkBlue = strip.Color(20, 67, 255); 
uint32_t brightRed = strip.Color(255, 20, 0); 

uint32_t lightBlue = strip.Color(20, 255, 243); 
uint32_t purple = strip.Color(145, 20, 255); 

uint32_t darkPink = strip.Color(237, 40, 184); 
uint32_t darkRed = strip.Color(165, 14, 14); 


// setup routine runs once when you press reset (only runs once)
void setup() {
    Serial.begin(9600);
    pinMode(button1,INPUT);
    pinMode(button2,INPUT);
    //pinMode(greenled,OUTPUT);
   

    strip.begin();
    strip.setBrightness(250); //sets max brightness for the LEDs (0 to 255)
    strip.show(); // Initialize all pixels to 'off'
}

// put your main code here, to run repeatedly
void loop(){
    // read the pushbutton input pin:
    button1State = digitalRead(button1);
    button2State = digitalRead(button2);
    // start timer
    currentMillis = millis();

  //button1 debounce
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button1State != lastButton1State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button1State == HIGH) {
        // increment counter to change the LED mode
        button1Counter++;
        Serial.print("1");
        Serial.println(button1Counter);
        button2Counter = 0;
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
        button2Counter++;
        Serial.print("2");
        Serial.println(button2Counter);
        button1Counter = 0;
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
    }
  }
  if(button1Counter == 1){    //jukebox hero
    //digitalWrite(greenled,HIGH);
    beatTime = beatFind(jukeBoxHero);
    loopTwo(beatTime, brightRed, darkRed, yellow);
  }
  if(button1Counter % 2 == 0){
    uint16_t i;
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, stripOff);
    }
    strip.show();
    button1Counter = 0;
  }
  if(button2Counter == 1) {   //shape of you
    beatTime = beatFind(shapeOfYou);
    loopTwo(beatTime, darkRed, yellow, darkBlue);
  }
  if(button2Counter % 2 == 0) {
     uint16_t i;
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, stripOff);
    }
    strip.show();
    button2Counter = 0;
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButton1State = button1State;
  lastButton2State = button2State;
}

double beatFind(double bpm) {
  double bps = bpm/60.0;
  double result = 1000.0/bps;
  return result;
}

void loopTwo(double msTime, uint32_t mainColor, uint32_t offColor,uint32_t mainColor2) {
  int i;
  for(i=0; i < 7; i++) {
    loopOne(msTime, mainColor, offColor);
  }
}

void loopOne(double msTime, uint32_t mainColor, uint32_t offColor) {
  colorMain(0, mainColor); //main color
  if (currentMillis - previousMillis >= (msTime-offColorDelay)) {
    colorOff(offColorDelay, offColor); //off color
    
    if(currentMillis - previousMillis >= msTime) {  
      previousMillis = currentMillis;
    }
  }
 }  

void colorOff(uint8_t beat, uint32_t color) {
  uint16_t i; 
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  //delay(beat);
}

void colorMain(uint8_t beat, uint32_t color) {
  uint16_t i;
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}



  



