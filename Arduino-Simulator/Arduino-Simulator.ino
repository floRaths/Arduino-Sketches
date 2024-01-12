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

  generateNewHues(pllt, 30, false);

  pllt.hueA = 30;
  pllt.hueB = 130;
  pllt.paletteType = "duotone";

  initializePerlin(500, 10000);

  scls.lumScales = {1000, 15000, 1000, 15000};
  scls.colScales = {1000, 15000, 1000, 15000};

  paletteSelection(pllt, pllt.paletteType);
  changeScales(scls, 6000, true, false);
  triggerBlend(50, false);
  blendColors(pllt, true, true, false);
}


// ################## LOOP ####################
void loop()
{
  int changes = 5000;

  EVERY_N_MILLISECONDS(changes)
  {
    generateNewHues(pllt, 30, true);
    // executed = false;

    Serial.println(pllt.hueA);
    Serial.println(pllt.hueB);

    // incrementPalette(pllt);
    paletteSelection(pllt, pllt.paletteType, true);
    triggerBlend(changes*0.99, true);
    // changeScales(scls, changes, false, false);
  }

  
  blendColors(pllt, true, true, false);
  makeNoise(pllt, hurry, true);
  FastLED.show();
}