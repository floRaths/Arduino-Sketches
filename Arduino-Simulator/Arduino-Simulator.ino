#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

const uint8_t MatrixX = 8;
const uint8_t MatrixY = 10;
#define NUM_LEDS MatrixX * MatrixY
#define LAST_LED 80
CRGB leds[NUM_LEDS];

const bool coil = true;
const bool flip = false;
const bool serpentine = false;
const bool prototyping = false;

uint8_t hurry = 4;
const char *paletteNames[] = {"monochrome", "duotone", "tricolore", "pastel", "pastelAccent", "static"};
const int  *brightnessVals[] = {255, 128, 86};

#include "auxFnss/auxFnss.h"

palette pllt;
scales  scls;
stripRange strp;

#include "buttons/buttons.h"

ramp indexRamp;

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

  // pllt.hueA = 30;
  // pllt.hueB = 150;
  pllt.paletteType = "tricolore";

  scls.colScales = {5000, 10000, 5000, 10000};
  scls.lumScales = {5000, 25000, 5000, 25000};

  strp.upper_limit = LAST_LED;
  strp.lower_limit = 0;

  initializePerlin(scls, 500, 10000);

  updatePalette   (pllt, pllt.paletteType, false, true, true);
  changeScales    (scls, 8000, false, false);
  changeBrightness(3500, false, brightnessVals[0], true);
  changeStripRange(strp, false, true, strp.upper_limit, strp.lower_limit, 5000);

  triggerBlend  (pllt, 50, true, false);
  blendColors   (pllt, true, false);

  //indexRamp.go(255, 20000, LINEAR, BACKANDFORTH);
}


// ################## LOOP ####################
void loop()
{
  int changes = 33000;

  EVERY_N_MILLISECONDS(changes)
  {
    updatePalette   (pllt, pllt.paletteType, false, true, true, true);
    triggerBlend    (pllt, changes*0.95, false, true);
    changeScales    (scls, changes, false, false);
  }

  //paletteIndex = indexRamp.update();

  blendColors(pllt, true, false);
  makeNoise(pllt, scls, hurry, true);
  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 10);
  FastLED.setBrightness(briRamp.update());
  // fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
  btn.tick();
}
