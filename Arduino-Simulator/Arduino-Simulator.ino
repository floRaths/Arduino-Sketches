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
  Serial.println();
  Serial.println("#### Palette Button ####");

  // reset any palette blending if needed
  blendRamp1.pause(); blendRamp1.go(0, 0);
  blendRamp2.pause(); blendRamp2.go(0, 0);

  changeBrightness(125, false, 0, true);

  generateNewHues(pllt, 30, true);
  updatePalette(pllt, pllt.paletteType, true, true, true, true);
  triggerBlend(pllt, 750, true, true);
  
  changeScales(scls, 2000, true, true);

  palette_changed = true;
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
  btn.attachLongPressStart(buttonHold);
  btn.setPressMs(250);

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

  triggerBlend  (pllt, 50, true, false);
  blendColors   (pllt, true, false);
}


// ################## LOOP ####################
void loop()
{
  int changes = 30000;

  EVERY_N_MILLISECONDS(changes)
  {
    updatePalette   (pllt, pllt.paletteType, false, true, true, true);
    triggerBlend    (pllt, changes*0.95, true, true);
    changeScales    (scls, changes, false, false);
  }

  uint16_t beatA = beatsin16(2, 0, 255);
  paletteIndex = beatA;

  blendColors(pllt, true, false);
  makeNoise(pllt, scls, hurry, true);
  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 10);
  FastLED.setBrightness(briRamp.update());
  fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
  btn.tick();
}
