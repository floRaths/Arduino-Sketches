#include <FastLED.h>

#define NUM_LEDS  150
#define LED_PIN   5

CRGB leds[NUM_LEDS];

CRGB source1[NUM_LEDS];
CRGB source2[NUM_LEDS];
CRGB output[NUM_LEDS];

uint8_t blendAmount = 0;
uint8_t patternCounter = 0;
uint8_t source1Pattern = 0;
uint8_t source2Pattern = 1;
bool useSource1 = false;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(output, NUM_LEDS);
  FastLED.setBrightness(50);
  Serial.begin(57600);
}

void loop() {

  EVERY_N_MILLISECONDS(10) {
    blend(source1, source2, output, NUM_LEDS, blendAmount);   // Blend between the two sources

    if (useSource1) {
      if (blendAmount < 255) blendAmount++;                   // Blend 'up' to source 2
    } else {
      if (blendAmount > 0) blendAmount--;                     // Blend 'down' to source 1
    }
  }

  EVERY_N_SECONDS(5) {
    nextPattern();
  }

  runPattern(source1Pattern, source1);                  // Run both patterns simultaneously
  runPattern(source2Pattern, source2);
  
  FastLED.show();
}

void nextPattern() {
  patternCounter = (patternCounter + 1) % 3;          // Change the number after the % to the number of patterns you have

  if (useSource1) source1Pattern = patternCounter;    // Determine which source array for new pattern
  else source2Pattern = patternCounter;

  useSource1 = !useSource1;                           // Swap source array for next time around
}

void runPattern(uint8_t pattern, CRGB *LEDArray) {
  switch (pattern) {
    case 0:
      Sparkle(LEDArray);
      break;
    case 1:
      rainbowBeat(LEDArray);
      break;
    case 2:
      redWhiteBlue(LEDArray);
      break;
  }
}





//------- Put your patterns below -------//

//----------------------------------------
uint8_t gHue = 0; // rotating “base color” used by many of the patterns

void Sparkle(CRGB *LEDarray) {
  
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 30 );
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(14), 0, 255 );
  //fadeToBlackBy(LEDarray, NUM_LEDS, 10);

}





void rainbowBeat(CRGB *LEDarray) {
  
  uint16_t beatA = beatsin16(30, 0, 255);
  uint16_t beatB = beatsin16(20, 0, 255);
  fill_rainbow(LEDarray, NUM_LEDS, (beatA+beatB)/2, 8);
}


void redWhiteBlue(CRGB *LEDarray) {

  uint16_t sinBeat   = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t sinBeat2  = beatsin16(30, 0, NUM_LEDS - 1, 0, 21845);
  uint16_t sinBeat3  = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);

  LEDarray[sinBeat]   = CRGB::Blue;
  LEDarray[sinBeat2]  = CRGB::Red;
  LEDarray[sinBeat3]  = CRGB::White;
  
  fadeToBlackBy(LEDarray, NUM_LEDS, 10);
}
