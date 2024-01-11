#include <FastLED.h>
#include <Ramp.h>

#define LED_PIN 5

const uint8_t kMatrixWidth = 15;
const uint8_t kMatrixHeight = 15;
boolean coil = true;
boolean flip = false;
boolean ser_col = false;
boolean prototyping = true;

#define NUM_LEDS kMatrixWidth *kMatrixHeight
CRGB leds[NUM_LEDS];

uint8_t hurry = 16;

#include "auxFnss/auxFnss.h"
#include "declarations/declarations.h"


// ################## SETUP ####################
void setup()
{
  Serial.begin(115200);
  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  generateNewHues(hueA, hueB, hueC, hueD, 30, false);

  lumRampX.go(500, 0, LINEAR);
  lumRampY.go(500, 0, LINEAR);
  colRampX.go(500, 0, LINEAR);
  colRampY.go(500, 0, LINEAR);

  lumScales = {1000, 15000, 1000, 15000};
  colScales = {1000, 15000, 1000, 15000};

  // random xy values for the noise field to ensure different starting points
  for (int i = 0; i < 4; i++)
  {
    xyVals[i] = random(10000);
  }
  
  changeScales(lumScales, colScales, 1000, true, false);
}

void loop()
{

  EVERY_N_MILLISECONDS(5000)
  {
    generateNewHues(hueA, hueB, hueC, hueD, 30, false);
    // this is a monochrome palette instruction
    colorType[0] = {hueA, 10, 100, 255, 155, 255};
    colorType[1] = {hueB, 10, 100, 255, 155, 255};
    colorType[2] = {hueA, 10, 100, 255, 155, 255};
    colorType[3] = {hueB, 10, 100, 255, 155, 255};

    triggerBlend(5000, false);
    changeScales(lumScales, colScales, 5000, false, false);
  }

  blendColors(runCol, newCol, colorType, true, true, false);

  makeNoise(runCol, hurry, true);
  //fill_palette(leds, NUM_LEDS, 0, 255 / NUM_LEDS, runPal, 255, LINEARBLEND);
  FastLED.show();
}