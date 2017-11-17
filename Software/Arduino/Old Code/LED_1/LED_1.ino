#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//Constants:
 double slow= 85.0;
 double medium = 105.0;
 double fast = 130.0;
 
 double jukeBoxHero = 177.0;
 double shapeOfYou = 96.0;
 
 int offColorDelay = 50; //(in ms) 10 works well but doesn't show the second color very much 

//Colors:
uint32_t turqoise = strip.Color(97, 232, 214); //creates a color
uint32_t magenta = strip.Color(235, 29, 29); //creates a color

uint32_t brightGreen = strip.Color(135, 255, 7); //creates a color
uint32_t yellow = strip.Color(255, 230, 12); //creates a color

uint32_t darkBlue = strip.Color(20, 67, 255); //creates a magenta color
uint32_t brightRed = strip.Color(255, 20, 0); //creates a magenta color

uint32_t lightBlue = strip.Color(20, 255, 243); //creates a magenta color
uint32_t purple = strip.Color(145, 20, 255); //creates a magenta color

uint32_t darkPink = strip.Color(237, 40, 184); //creates a magenta color
uint32_t darkRed = strip.Color(165, 14, 14); //creates a magenta color

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(9600); // start the serial port
  //initialize the LED strip
  strip.begin();
  strip.setBrightness(150); //sets max brightness for the LEDs (0 to 255)
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {
  double beatTime = beatFind(fast);
  loopTwo(beatTime, darkBlue, brightRed, yellow);
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
  loopOne(msTime, mainColor2, offColor);
}

void loopOne(double msTime, uint32_t mainColor, uint32_t offColor) {
  colorMain(0, mainColor); //main color
  delay(msTime-offColorDelay);
  colorOff(offColorDelay, offColor); //off color
}

void colorOff(uint8_t beat, uint32_t color) {
  uint16_t i; 
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(beat);
}

void colorMain(uint8_t beat, uint32_t color) {
  uint16_t i;
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(beat);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
