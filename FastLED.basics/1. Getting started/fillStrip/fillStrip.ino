#include <FastLED.h>

#define NUM_LEDS  10
#define LED_PIN   5

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(Tungsten40W);
  FastLED.setBrightness(100);
}

void loop() {

  uint8_t mappedValue = map(inoise8(millis()/7), 0, 255, 50, 150);
  
  CHSV hsvColor(120, mappedValue, 255); // Green color

  // Convert CHSV color to CRGB
  CRGB rgbColor = hsvColor;
  
  fill_solid(leds, NUM_LEDS, rgbColor);
  FastLED.show();
  // delay(500);
  
  // fill_solid(leds, NUM_LEDS, CRGB::Green);
  // FastLED.show();
  // delay(500);
  
  // fill_solid(leds, NUM_LEDS, CRGB::Blue);
  // FastLED.show();
  // delay(500);

  // fill_gradient_RGB(leds, NUM_LEDS, CRGB::Magenta, CRGB::Yellow);
  // FastLED.show();
  // delay(500);

  // fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Blue);
  // FastLED.show();
  // delay(500);

  // fill_rainbow(leds, NUM_LEDS, 0, 255 / NUM_LEDS);
  // FastLED.show();
  // delay(500);
}
