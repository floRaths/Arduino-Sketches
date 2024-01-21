#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

const uint8_t MatrixX = 8;
const uint8_t MatrixY = 10;
#define NUM_LEDS MatrixX * MatrixY
#define LAST_LED NUM_LEDS
CRGB leds[NUM_LEDS];

const bool coil = true;
const bool flip = false;
const bool serpentine = false;
const bool prototyping = false;
const int  xOffset = -4;

uint8_t hurry = 4;
//const char *paletteNames[] = {"monochrome", "duotone", "tricolore", "pastel", "pastelAccent", "static"};
const char *paletteNames[] = {"monochrome", "duotone", "tricolore", "pastel", "pastelAccent", "static"};
const int  *brightnessVals[] = {255, 128, 86};

#include "auxFnss/auxFnss.h"

palette pllt;
scales  scls;
stripRange strp;

#include "buttons/buttons.h"

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

  pllt.hueA = 48;
  pllt.hueB = 146;
  pllt.hueC = 199;
  pllt.paletteType = "tricolore";

  scls.colScales = {5000, 25000, 5000, 25000};
  scls.lumScales = {5000, 50000, 5000, 50000};

  strp.upper_limit = LAST_LED;
  strp.lower_limit = 0;

  initializePerlin(scls, 500, 10000);

  updatePalette   (pllt, pllt.paletteType, false, false, true);
  changeScales    (scls, 8000, false, false);
  changeBrightness(3500, false, brightnessVals[0], true);
  changeStripRange(strp, false, true, strp.upper_limit, strp.lower_limit, 5000);

  triggerBlend  (pllt, 50, true, true, true);
  blendColors   (pllt);

  indexRamp.go(255, 25000, LINEAR, BACKANDFORTH);
}


// ################## LOOP ####################
void loop()
{
  int changes = 35;

  EVERY_N_SECONDS(changes)
  {
    Serial.println();
    Serial.println("######## Periodic Rendomizaiton ########");
    updatePalette (pllt, pllt.paletteType, false, false, true, true);
    triggerBlend  (pllt, changes * 900, true, true, true);
    changeScales  (scls, changes * 900, false, true);
  }

  paletteIndex = indexRamp.update();

  blendColors(pllt, true);
  makeNoise(pllt, scls, hurry, true);
  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 10);
  FastLED.setBrightness(briRamp.update());
  // fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
  btn.tick();
}
