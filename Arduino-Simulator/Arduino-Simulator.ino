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
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  generateNewHues(hueA, hueB, hueC, hueD, 30, false);

  initializePerlin(500, 10000);

  scales.lumScales = {1000, 15000, 1000, 15000};
  scales.colScales = {1000, 15000, 1000, 15000};

  pal.col[0] = {hueA, 10, 155, 255, 155, 255};
  pal.col[1] = {hueB, 10, 155, 255, 155, 255};
  pal.col[2] = {hueA, 10, 1, 5, 155, 255};
  pal.col[3] = {hueB, 10, 1, 5, 155, 255};

  changeScales(scales, 6000, true, false);
  triggerBlend(50, false);
  blendColors(pal, true, true, false);  
}

void loop()
{
  EVERY_N_MILLISECONDS(2000)
  {
    generateNewHues(hueA, hueB, hueC, hueD, 30, false);
    
    pal.col[0] = {hueA, 10, 155, 255, 155, 255};
    pal.col[1] = {hueB, 10, 155, 255, 155, 255};
    pal.col[2] = {hueA, 10, 1, 5, 155, 255};
    pal.col[3] = {hueB, 10, 1, 5, 155, 255};

    triggerBlend(1900, false);
    changeScales(scales, 4000, false, false);
  }

  blendColors(pal, true, true, false);
  makeNoise(pal, hurry, true);
  FastLED.show();
}