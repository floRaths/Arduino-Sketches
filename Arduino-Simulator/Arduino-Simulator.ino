#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

const uint8_t MatrixX = 10;
const uint8_t MatrixY = 10;
#define NUM_LEDS MatrixX * MatrixY
#define LAST_LED 100
CRGB leds[NUM_LEDS];

const bool coil = true;
const bool flip = false;
const bool serpentine = false;
const bool prototyping = false;

uint8_t hurry = 6;
const char *paletteNames[] = {"monochrome", "duotone", "tricolore", "pastel", "pastelAccent", "static"};
const int  *brightnessVals[] = {255, 128, 86};

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
  updatePalette(pllt, pllt.paletteType, true, true);
  triggerBlend(500, true);
  //blendColors(pllt, true, true, false);
}

// ################## SETUP ####################
void setup()
{
  delay(100); // startup safety delay  
  Serial.begin(9600);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(Tungsten40W);
  FastLED.setBrightness(0);

  btn.attachClick(buttonClick);
  //btn.attachLongPressStart(buttonHold);

  Serial.println();
  Serial.println("######## Hello Lamp ########");
  Serial.println("############################");

  pllt.hueA = 30;
  pllt.hueB = 50;
  pllt.paletteType = "duotone";

  scls.colScales = {1000, 10000, 1000, 10000};
  scls.lumScales = {5000, 15000, 5000, 15000};

  strp.upper_limit = LAST_LED;
  strp.lower_limit = 0;

  initializePerlin(scls, 500, 10000);

  updatePalette   (pllt, pllt.paletteType);
  changeScales    (scls, 6000, false, false);
  changeBrightness(2500, false, brightnessVals[0], true);
  changeStripRange(strp, false, true, strp.upper_limit, strp.lower_limit, 3000);

  triggerBlend  (50, false);
  blendColors   (pllt, true, true, false);
}


// ################## LOOP ####################
void loop()
{
  int changes = 30000;

  EVERY_N_MILLISECONDS(changes)
  {
    updatePalette   (pllt, pllt.paletteType, false, false, true, true);
    triggerBlend    (changes*0.95, true);
    changeScales    (scls, changes, false, false);
  }

  blendColors(pllt, true, true, false);
  makeNoise(pllt, scls, hurry, true);
  //showCenter();
  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 10);
  FastLED.setBrightness(briRamp.update());
  //fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
  btn.tick();
}


