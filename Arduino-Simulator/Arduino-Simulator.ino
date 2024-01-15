#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

const uint8_t MatrixX = 16;
const uint8_t MatrixY = 16;
#define NUM_LEDS MatrixX * MatrixY
#define LAST_LED 255
CRGB leds[NUM_LEDS];

const bool coil = false;
const bool flip = false;
const bool serpentine = true;
const bool prototyping = true;

uint8_t hurry = 6;
const char *paletteNames[] = {"monochrome", "duotone", "pastel", "pastelAccent", "static"};
const int  *brightnessVals[] = {255, 128, 128};

#include "auxFnss/auxFnss.h"
#include "palettes/palettes.h"


palette pllt;
scales  scls;
stripRange strp;


void buttonClick() {
  changeBrightness(1000, true, 255, true);
  changeStripRange(strp, true, true);
}

void buttonHold() {
  generateNewHues(pllt, 30, true);
  changePalette(pllt, pllt.paletteType, true, true);
  triggerBlend(500, true);
  //blendColors(pllt, true, true, false);
}

// ################## SETUP ####################
void setup()
{
  delay(100); // startup safety delay  
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  // FastLED.setCorrection(TypicalLEDStrip);
  // FastLED.setTemperature(Tungsten40W);
  FastLED.setBrightness(0);

  btn.attachClick(buttonClick);
  btn.attachLongPressStart(buttonHold);

  Serial.println();
  Serial.println("######## Hello Lamp ########");
  Serial.println("############################");

  pllt.hueA = 250;
  pllt.hueB = 60;
  pllt.paletteType = "duotone";

  scls.lumScales = {5000, 10000, 5000, 10000};
  scls.colScales = {5000, 10000, 5000, 10000};

  strp.upper_limit = LAST_LED;
  strp.lower_limit = 0;

  initializePerlin(scls, 500, 10000);

  changePalette    (pllt, pllt.paletteType);
  changeScales     (scls, 6000, true, false);
  changeBrightness (2500, false, 255, true);
  changeStripRange (strp, false, true, strp.upper_limit, strp.lower_limit, 3000, 3000);

  triggerBlend  (50, false);
  blendColors   (pllt, true, true, false);
}


// ################## LOOP ####################
void loop()
{
  int changes = 5000;

  EVERY_N_MILLISECONDS(changes)
  {
    //generateNewHues (pllt, 30, false);
    //changePalette   (pllt, pllt.paletteType, false, false);
    //triggerBlend    (changes*0.95, false);
    //changeScales    (scls, changes, false, false);
  }

  blendColors(pllt, true, true, false);
  makeNoise(pllt, scls, hurry, true);

  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 8);
  FastLED.setBrightness(briRamp.update());
  //fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
  btn.tick();
}