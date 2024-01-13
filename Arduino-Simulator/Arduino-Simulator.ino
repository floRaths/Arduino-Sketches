#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

const uint8_t MatrixX = 16;
const uint8_t MatrixY = 16;
#define NUM_LEDS MatrixX * MatrixY
CRGB leds[NUM_LEDS];

const bool coil = false;
const bool flip = false;
const bool serpentine = true;
const bool prototyping = true;

uint8_t hurry = 6;
const char *paletteNames[] = {"monochrome", "duotone", "pastel", "pastelAccent", "static"};
const int  *brightnessVals[] = {255, 128, 0};

#include "auxFnss/auxFnss.h"
#include "palettes/palettes.h"

palette pllt;
scales  scls;

void buttonClick() {
  changeBrightness(1000, true, 255, true);
}

// ################## SETUP ####################
void setup()
{
  delay(100); // startup safety delay  
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(Tungsten40W);
  FastLED.setBrightness(0);

  btn.attachClick(changeBrightness);

  Serial.println();
  Serial.println("######## Hello Lamp ########");
  Serial.println("############################");

  pllt.hueA = 250;
  pllt.hueB = 60;
  pllt.paletteType = "pastel";

  initializePerlin(scls, 500, 10000);

  scls.lumScales = {1000, 15000, 1000, 15000};
  scls.colScales = {1000, 5000, 1000, 5000};

  changePalette (pllt, pllt.paletteType);
  changeScales  (scls, 6000, true, false);
  changeBrightness(2500, false, 255, true);
  
  triggerBlend  (50, false);
  blendColors   (pllt, true, true, false);
}


// ################## LOOP ####################
void loop()
{
  int changes = 5000;

  EVERY_N_MILLISECONDS(changes)
  {
    //generateNewHues (pllt, 30, true);
    changePalette   (pllt, pllt.paletteType, true, true);
    triggerBlend    (changes*0.95, true);
    changeScales    (scls, changes, false, false);
    //changeBrightness(250, true, 255, true);

  }

  blendColors(pllt, true, true, true);
  makeNoise(pllt, scls, hurry, true);
  FastLED.setBrightness(briRamp.update());
  FastLED.show();
  btn.tick();
}