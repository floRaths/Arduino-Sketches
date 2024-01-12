#include <FastLED.h>
#include <Ramp.h>

#define LED_PIN 5

const uint8_t MatrixX = 16;
const uint8_t MatrixY = 16;
#define NUM_LEDS MatrixX * MatrixY
CRGB leds[NUM_LEDS];

boolean coil = true;
boolean flip = false;
boolean ser_col = false;
boolean prototyping = true;

uint8_t hurry = 16;
const char *paletteNames[] = {"monochrome", "duotone", "pastel", "static"};

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
  
  Serial.println();
  Serial.println("######## Hello Lamp ########");
  Serial.println("############################");

  pllt.hueA = 230;
  pllt.hueB = 130;
  pllt.paletteType = "duotone";

  initializePerlin(scls, 500, 10000);

  scls.lumScales = {10000, 15000, 10000, 15000};
  scls.colScales = {1000, 5000, 1000, 5000};

  updatePalette (pllt, pllt.paletteType);
  changeScales  (scls, 6000, true, false);
  triggerBlend  (50, false);
  blendColors   (pllt, true, true, false);
}


// ################## LOOP ####################
void loop()
{
  int changes = 4000;

  EVERY_N_MILLISECONDS(changes)
  {
    // generateNewHues (pllt, 30, true);
    updatePalette   (pllt, pllt.paletteType, false, true);
    triggerBlend    (changes*0.95, true);
    changeScales    (scls, changes, true, false);
  }

  blendColors(pllt, true, true, false);
  makeNoise(pllt, scls, hurry, true);
  FastLED.show();
}