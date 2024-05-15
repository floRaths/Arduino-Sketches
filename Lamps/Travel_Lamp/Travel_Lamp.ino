#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

const uint8_t MatrixX = 10;
const uint8_t MatrixY = 10;
#define NUM_LEDS MatrixX * MatrixY
#define LAST_LED NUM_LEDS
CRGB leds[NUM_LEDS];

const bool coil = true;
const bool flip = false;
const bool serpentine = false;
const bool prototyping = false;
const uint8_t  xOffset = 0;

uint8_t hurry = 6;
//const char *paletteNames[] = {"monochrome", "duotone", "tricolore", "pastel", "pastelAccent", "static"};
const char *paletteNames[] = {"monochrome", "duotone"};
//const int  *brightnessVals[] = {255, 200, 128};
//const int  *brightnessVals[] = {255, 255, 255, 128, 128, 128, 64, 64, 64};
const int  *brightnessVals[] = {255, 128, 64, 255, 128, 64, 255, 128, 64};

#include "auxFnss/auxFnss.h"

palette pllt;
scales  scls;
stripRange strp;

// rampFloat speedRamp;
// float targetSpeed = 1.0;

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

  btn.attachClick(userBrightness);
  btn.attachLongPressStart(userPalette);
  btn.attachDoubleClick(userBlink);
  btn.setPressMs(250);

  Serial.println();
  Serial.println("######## Hello Lamp ########");
  Serial.println("############################");

  pllt.hueA = 30;
  pllt.hueB = 50;
  pllt.paletteType = "startUp";

  scls.colScales = {2500, 15000, 2500, 15000};
  scls.lumScales = {5000, 18000, 5000, 18000};

  strp.upper_limit = LAST_LED;
  strp.lower_limit = 0;
  strp.upper_store = strp.upper_limit;
  strp.lower_store = strp.lower_limit;

  initializePerlin(scls, 500, 10000);

  // generateNewHues (pllt, 30, true, true);
  updatePalette   (pllt, pllt.paletteType, false, true);
  changeScales    (scls, 15000, true, true);
  changeBrightness(3500, false, brightnessVals[0], true);
  changeStripRange(strp, false, true, strp.upper_limit, strp.lower_limit, 5000);

  triggerBlend  (pllt, 50, true, true);
  blendColors   (pllt);

  // speedRamp.go(targetSpeed, 1000, CIRCULAR_INOUT);
}





// ################## LOOP ####################
void loop()
{
  if (anythingGoes == true)
  {
    EVERY_N_SECONDS(35)
    {
      Serial.println();
      Serial.println("######## Introducing New Hues ########");
      generateNewHues(pllt, 15, true, true);
      if (pllt.paletteType == "monochrome")
      {
        updatePalette(pllt, pllt.paletteType, false, true, true);
      }
      else
      {
        updatePalette(pllt, pllt.paletteType, false, false, true);
      }
      triggerBlend(pllt, 30000, true, true);
    }
  }

  EVERY_N_SECONDS(26)
  {
    Serial.println();
    Serial.println("######## Palette Randomizaiton ########");
    updatePalette(pllt, pllt.paletteType, false, false, true);
    triggerBlend(pllt, 25000, true, true);
  }


  if (isPressed == true) {
    if(briRamp.isFinished() == true && strp.lowerRamp.isFinished() == true) {
      changeBrightness(1000, false, brightnessVals[switchBrightness], true);
      changeStripRange(strp, false, true, strp.upper_store, strp.lower_store, 1500);
      isPressed = false;
    }
  }

  EVERY_N_SECONDS(17)
  {
    Serial.println();
    Serial.println("######## Scale Randomizaiton ########");
    changeScales(scls, 15000, true, true);
  }

  paletteIndex = map(inoise16(millis()*5), 0, 65535, 0, 255);

  blinkAction();

  blendColors(pllt, true);
  makeNoise(pllt, scls, hurry, true);
  //showCenter(xOffset);
  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 10);
  FastLED.setBrightness(briRamp.update());
  // fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
  btn.tick();
}
