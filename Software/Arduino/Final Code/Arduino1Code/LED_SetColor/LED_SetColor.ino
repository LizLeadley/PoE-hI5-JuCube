#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

uint32_t hotPink = strip.Color(255, 25, 128); 
uint32_t black = strip.Color(0, 0, 0); 
double pink_r_g = 25.0/255.0;
double pink_r_b = 128.0/255.0;
uint32_t blue = strip.Color(35, 73, 224);
double blue_b_r = 35.0/224.0; //blue ratio for blue to red
double blue_b_g = 73/224.0;  //blue ratio for blue to green
int colorVar = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //initialize serial
  
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(240);
  //setColor(blue);
  doubleColor(hotPink, blue);
}

void loop() {

}

void doubleColor(uint32_t colorOne, uint32_t colorTwo) {
  uint16_t i;
  for(i=0; i<strip.numPixels(); i++) {
    if ((i % 2) == 0) {
      strip.setPixelColor(i, colorOne);
    }
    else {
      strip.setPixelColor(i, colorTwo);
    }
    strip.show();
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  uint16_t j;
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i + 1, c);
      //strip.setPixelColor(i, black);
    strip.show();
    delay(wait);
  }
}


void colorWipeFlip(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels() + 2; i++) {
      strip.setPixelColor(strip.numPixels() - (i+1), c);
      //strip.setPixelColor(strip.numPixels() - (i), black);
    strip.show();
    delay(wait);
  }
}
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void setColor(uint32_t color) {
  uint16_t i;
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    //strip.setPixelColor(i, Wheel( (i) % 255));    //turn every third pixel on
  }
  strip.show();
}

void splitColor(uint32_t color1, uint32_t color2) {
  uint16_t i;
  for(i=0; i<strip.numPixels()/2; i++) {
    strip.setPixelColor(i, color1);
  }
  for(i=strip.numPixels()/2; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color2);
  }
  strip.show();
}

