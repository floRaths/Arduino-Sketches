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
const int  xOffset = -5;

uint8_t hurry = 10;
//const char *paletteNames[] = {"monochrome", "duotone", "tricolore", "pastel", "pastelAccent", "static"};
const char *paletteNames[] = {"monochrome", "duotone"};
const int  *brightnessVals[] = {128, 64, 255};

//#include "palette.h"
#include "auxFnss.h"

palette pllt;
scales  scls;
stripRange strp;

rampFloat speedRamp;
float targetSpeed = 10.0

#include "buttons.h"

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
  pllt.hueB = 150;
  
  pllt.paletteType = "duotone";

//   scls.colScales = {2500, 15000, 2500, 15000};
//   scls.lumScales = {5000, 18000, 5000, 18000};
  scls.colScales = {12500, 15000, 12500, 15000};
  scls.lumScales = {15000, 18000, 15000, 18000};

  strp.upper_limit = LAST_LED;
  strp.lower_limit = 0;

  initializePerlin(scls, 500, 10000, true);

  // generateNewHues (pllt, 30, true, true);
  updatePalette   (pllt, pllt.paletteType, false, true);
  changeScales    (scls, 500, true, true);
  changeBrightness(3500, false, brightnessVals[0], true);
  changeStripRange(strp, false, true, strp.upper_limit, strp.lower_limit, 500);

  triggerBlend  (pllt, 50, true, true);
  blendColors   (pllt);

  paletteIndex = 180;
  speedRamp.go(targetSpeed, 1000, CIRCULAR_INOUT)

}


// ################## LOOP ####################
void loop()
{

  EVERY_N_MILLISECONDS(1000) {
    float targetSpeed = targetSpeed+5.5
    speedRamp.go(10.0, 1000, CIRCULAR_INOUT)
    //paletteIndex = paletteIndex + 1;
    //Serial.println(paletteIndex);
  }
  
  blendColors(pllt, true);
  makeNoise(pllt, scls, hurry, true);
  //showCenter(xOffset);
  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 10);
  FastLED.setBrightness(briRamp.update());
  //fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
  btn.tick();
}
