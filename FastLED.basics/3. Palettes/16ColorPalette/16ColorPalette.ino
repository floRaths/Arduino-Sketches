#include <FastLED.h>

#define NUM_LEDS  75
#define LED_PIN   5

CRGB leds[NUM_LEDS];

uint8_t paletteIndex = 0;

CRGBPalette16 purplePalette = CRGBPalette16 (
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::DarkViolet,
    
    CRGB::Magenta,
    CRGB::Magenta,
    CRGB::Linen,
    CRGB::Linen,
    
    CRGB::Magenta,
    CRGB::Magenta,
    CRGB::DarkViolet,
    CRGB::DarkViolet,

    CRGB::DarkViolet,
    CRGB::DarkViolet,
    CRGB::Linen,
    CRGB::Linen
);

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
}

void loop() {

  uint8_t mappedh1 = map8(inoise8(millis()/2), 0, 100);
  uint8_t mappedh2 = map8(inoise8(millis()/3), 100, 150);
  // uint8_t mappedValue = map(inoise8(millis()/7), 0, 255, 50, 150);
  // uint8_t mappedValue = map(inoise8(millis()/7), 0, 255, 50, 150);
  
  CHSV color1 = CHSV(mappedh1, 255, 255);
  CHSV color2 = CHSV(mappedh2, 255, 255);

  CRGBPalette16 runPal = CRGBPalette16(color1, color1, color2, color2);
  
  fill_palette(leds, NUM_LEDS, 0, 3, runPal, 255, LINEARBLEND);
  
  // EVERY_N_MILLISECONDS(10){
  //   paletteIndex++;
  // }
  
  FastLED.show();
}
