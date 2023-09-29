#include <FastLED.h>

#define NUM_LEDS  150
#define LED_PIN   5

CRGB leds[NUM_LEDS];

uint8_t patternCounter = 0;
uint8_t paletteIndex = 0;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  Serial.begin(57600);
}


  

void loop() {
  switch (patternCounter) {
    case 0:
      BounceDot();
      break;        
  }

  EVERY_N_SECONDS(32) {
    nextPattern();
  }
  
  FastLED.show();
}


void nextPattern() {
  patternCounter = (patternCounter + 1) % 1;          // Change the number after the % to the number of patterns you have
}





//----------------------------------------
void BounceDot() {

  // Waves for LED position
  uint16_t posBeat  = beatsin16(11, 0, NUM_LEDS, 0, 0);
  uint16_t posBeat2 = beatsin16(19, 0, NUM_LEDS, 0, 20000);
  uint16_t posBeat3 = beatsin16(25, 0, NUM_LEDS, 0, 10000);
  uint16_t posBeat4 = beatsin16(9,  0, NUM_LEDS, 0, 32767);

  // Wave for LED color
  uint8_t colBeat  = beatsin8(45, 135, 225, 0, 0);

  // Wave for Fade 
    uint8_t fadeBeat  = beatsin8(18, 0, 20, 0, 0);

  leds[(posBeat  + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, fadeBeat);
}
