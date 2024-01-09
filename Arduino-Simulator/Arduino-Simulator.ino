#include <FastLED.h>
#include "auxFnss/auxFnss.h"

// Pin definitions
#define LED_PIN 5
#define NUM_LEDS 104
CRGB leds[NUM_LEDS];


CRGB runCol[4];

uint8_t hueA, hueB, hueC, hueD;

// ################## SETUP ####################
void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}


void loop()
{
  EVERY_N_MILLISECONDS(1500)
  {
    generateNewHues(hueA, hueB, hueC, hueD, 30, true);

    // this is a monochrome palette instruction
    color[0] = {hueA, 0, 55, 255, 155, 255};
    color[1] = {hueB, 0, 55, 255, 155, 255};
    color[2] = {hueC, 0, 55, 255, 155, 255};
    color[3] = {hueD, 0, 55, 255, 155, 255};

    assemblePalette(true, true);

    CRGBPalette16 runPal = CRGBPalette16(newCol[0], newCol[1], newCol[2], newCol[3]);

    fill_palette(leds, NUM_LEDS, 0, 255 / NUM_LEDS, runPal, 255, LINEARBLEND);
    FastLED.show();
  }
}
