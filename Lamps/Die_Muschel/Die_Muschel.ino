
#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>
//#include <Adafruit_DotStar.h>

//Adafruit_DotStar strip(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);

// Pin definitions
#define LED_PIN 5
#define BTN_PIN 7

// Button Initialization
OneButton btn = OneButton(BTN_PIN, true, true);

// ################## matrix ###################
const uint8_t MatrixX = 10;
const uint8_t MatrixY = 10;
#define NUM_LEDS MatrixX * MatrixY
#define LAST_LED NUM_LEDS
CRGB leds[NUM_LEDS];

const bool coil = true;
const bool flip = false;
const bool serpentine = true;
const bool prototyping = false;
const int  xOffset = 0;

uint8_t hurry = 26;

const String paletteNames[] = {"duotone", "pastel"};
uint8_t Intensities[] = {255, 100, 20};
const int brightnessVals[] = {255, 86, 86};


#include "auxFnss.h"

palette pllt;
scales  scls;
stripRange strp;

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

  btn.attachClick(userPalette);
  btn.attachDoubleClick(userBrightness);
  btn.attachLongPressStart(userIntensity);
  btn.setPressMs(250);

  Serial.println();
  Serial.println("######## Hello Lamp ########");
  Serial.println("############################");

  pllt.hueA = 30;
  pllt.hueB = 50;
  // pllt.hueC = 170;
  pllt.paletteType = "duotone";

  scls.colScales = {2500, 15000, 2500, 15000};
  //scls.lumScales = {5000, 18000, 5000, 18000};
  // scls.colScales = {15000, 18000, 15000, 18000};
  scls.lumScales = {15000, 18000, 15000, 18000};

  strp.upper_limit = LAST_LED - 4;
  strp.lower_limit = 0;

  initializePerlin(scls, 500, 10000);

  // generateNewHues (pllt, 30, true, true);
  updatePalette   (pllt, pllt.paletteType, false, 255, true);
  changeScales    (scls, 15000, true, true);
  changeBrightness(3500, false, brightnessVals[0], true);
  changeStripRange(strp, false, true, strp.upper_limit, strp.lower_limit, 5000);

  triggerBlend  (pllt, 50, true, true);
  blendColors   (pllt);

  //indexRamp.go(255, 25000, LINEAR, BACKANDFORTH);
}



// ################## LOOP ####################
void loop()
{ 
  btn.tick();

  // if (anythingGoes == 2 && has_been_pressed)
  // {
  //   EVERY_N_SECONDS(180)
  //   {
  //     Serial.println();
  //     Serial.println("######## Introducing New Hues ########");
  //     generateNewHues(pllt, 15, true, true);
  //     updatePalette(pllt, pllt.paletteType, false, true, true);
  //     triggerBlend(pllt, 25000, true, true);
  //   }
  // }

  // EVERY_N_SECONDS(16)
  // {
  //   Serial.println();
  //   Serial.println("######## Palette Randomizaiton ########");
  //   generateNewHues(pllt, 15, true, true);
  //   updatePalette(pllt, pllt.paletteType, false, false, true);
  //   triggerBlend(pllt, 10000, true, true);
  // }

  // EVERY_N_SECONDS(70)
  // {
  //   Serial.println();
  //   Serial.println("######## Scale Randomizaiton ########");
  //   changeScales(scls, 25000, true, true);
  // }

  //paletteIndex = indexRamp.update();

  blendColors(pllt, true);
  makeNoise(pllt, scls, hurry, true);
  //showCenter(xOffset);
  updateRange(strp.lowerRamp.update(), strp.upperRamp.update(), 10);
  FastLED.setBrightness(briRamp.update());
  //fadeToBlackBy(leds, NUM_LEDS, 64);
  FastLED.show();
}
