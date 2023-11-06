#include <FastLED.h>

#define NUM_LEDS  104
#define LED_PIN   5

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {
  fill_solid(leds, NUM_LEDS, CRGB::White);
  
  FastLED.setCorrection(UncorrectedColor);
  leds[30] = CRGB::Red;
  FastLED.show();
  delay(2000);

  FastLED.setCorrection(TypicalLEDStrip);
  leds[30] = CRGB::Green;
  FastLED.show();
  delay(2000);

  FastLED.setCorrection(TypicalPixelString);
  leds[30] = CRGB::Blue;
  FastLED.show();
  delay(2000);
  
}
