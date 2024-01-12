#include <FastLED.h>
#include <Ramp.h>

#define LED_PIN 5

const uint8_t MatrixX = 15;
const uint8_t MatrixY = 15;
#define NUM_LEDS MatrixX * MatrixY
CRGB leds[NUM_LEDS];

boolean coil = true;
boolean flip = false;
boolean ser_col = false;
boolean prototyping = true;

uint8_t hurry = 16;
const char *paletteNames[] = {"monochrome", "duotone", "pastel", "whacky"};

#include "auxFnss/auxFnss.h"
#include "palettes/palettes.h"

palette pllt;
scales  scls;

// ################## SETUP ####################
void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  
  Serial.println(); Serial.println("##### Hello Lamp #####");
  
  generateNewHues(pllt, 30, false);

  pllt.hueA = 20;

  initializePerlin(500, 10000);

  scls.lumScales = {1000, 15000, 1000, 15000};
  scls.colScales = {1000, 15000, 1000, 15000};

  paletteSelection(pllt, "monochrome");
  changeScales(scls, 6000, true, false);
  triggerBlend(50, false);
  blendColors(pllt, true, true, false);
}


// ################## LOOP ####################
void loop()
{
  EVERY_N_MILLISECONDS(6000)
  {
    generateNewHues(pllt, 30, true);
    Serial.println(pllt.hueA);
    incrementPalette(pllt);
    triggerBlend(500, true);
    changeScales(scls, 4000, false, false);
  }

  paletteSelection(pllt, pllt.paletteType, true);
  blendColors(pllt, true, true, false);
  makeNoise(pllt, hurry, true);
  FastLED.show();
}