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
      paletteSparkle();
      break;
    case 1:
      fire();
      break;
  }

  EVERY_N_SECONDS(30) {
    nextPattern();
  }
  
  FastLED.show();
}

void nextPattern() {
  patternCounter = (patternCounter + 1) % 1;          // Change the number after the % to the number of patterns you have
}


uint8_t gHue = 0; // rotating “base color” used by many of the patterns


//------- Put your patterns below -------//

void paletteSparkle()  {
  
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 30 );
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(14), 0, 255 );

}


void fire() {

  {
// FastLED’s built-in rainbow generator
fill_rainbow( leds, NUM_LEDS, gHue, 25);
}
}
