#include <FastLED.h>
#include <Ramp.h>
#include "auxFnss/auxFnss.h"
#include "declarations/declarations.h"

// ################## SETUP ####################
void setup()
{
  Serial.begin(115200);
  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
}

void loop()
{
  EVERY_N_MILLISECONDS(2000)
  {
    generateNewHues(hueA, hueB, hueC, hueD, 30, false);

    // this is a monochrome palette instruction
    colorType[0] = {hueA, 10, 155, 255, 155, 255};
    colorType[1] = {hueA, 10, 155, 255, 155, 255};
    colorType[2] = {hueB, 10, 155, 255, 155, 255};
    colorType[3] = {hueB, 10, 155, 255, 155, 255};

    assemblePalette(newCol, colorType, true, false);
    triggerBlend(1000, true);
  }

  blendColors(runCol, newCol, true);

  CRGBPalette16 runPal = CRGBPalette16(runCol[0], runCol[1], runCol[2], runCol[3]);
  fill_palette(leds, NUM_LEDS, 0, 255 / NUM_LEDS, runPal, 255, LINEARBLEND);
  FastLED.show();
}