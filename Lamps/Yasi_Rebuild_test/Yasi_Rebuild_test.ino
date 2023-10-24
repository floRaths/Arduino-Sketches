#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

// Pin definitions
#define LED_PIN 5
#define BTN_PIN 7

// Button Initialization
OneButton btn = OneButton(BTN_PIN, true, true);

// #############################################
// ################## SETUP ####################

// ################## matrix ###################
const uint8_t kMatrixWidth  = 8;
const uint8_t kMatrixHeight = 13;

#define NUM_LEDS kMatrixWidth *kMatrixHeight
CRGB leds[NUM_LEDS];

boolean coil = true;
boolean flip = false;
boolean ser_col = true;
//boolean prototyping = false;

// ################## config ###################
uint8_t hurry = 2;

// three brightness values to choose via button
uint8_t Bri1 = 255;
uint8_t Bri2 = 75;
uint8_t Bri3 = 75;

// prameters for initial palette selection
uint8_t base_hue1 = 20;  // first hue
uint8_t base_hue2 = 160; // second hue
uint8_t range = 0;       // fluctuation

// parameter for moving the lit area
uint16_t lower = 0;        // lower end of lights
uint16_t upper = NUM_LEDS - 5; // upper end of lights
uint16_t store;

// ################## inits ###################
// booleans for conditional execution
boolean start_up = false; // allows a small startup sequence
boolean palette_changed = false;
boolean grant_blend = false;
boolean new_colors = false;

uint8_t paletteIndex, CurrentBri, coin;
int speed1, speed2; // initial smoothing speeds set (0.5 seconds)
uint32_t xyVals[4];
int noiRampMin[4];
int noiRampMax[4];

CRGB col[4]; // these are the currently used colors
CRGB pal[4]; // these are colors of a new palette that we blend to

// initializing smoothing functions
ramp briRamp, palRamp1, palRamp2; // smooth palette blending 1
rampInt lowerRamp, upperRamp; // smooth area blending 1
rampLong lumRampX, lumRampY, colRampX, colRampY; // smooth luminance scale blending on X
rampLong timeRamp;

// #############################################
// ################## SETUP ####################
void setup() {
  
  delay(200); // startup safety delay
  Serial.begin(115200);
  randomSeed(analogRead(0) * 100);

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setBrightness(255); // this brightness will be overridden by makeNoise function
  FastLED.setDither(0);

  btn.attachClick(brightnessAreaButton);
  btn.attachLongPressStart(paletteButton);
  btn.setPressMs(250);

  CurrentBri = Bri1;

  // random xy values for the noise field to ensure different starting points
  for (int i = 0; i < 4; i++)
  {
    noiRampMin[i] = 5000;//random(5000, 7000);
    noiRampMax[i] = 5000;//random(7000, 15000);
    xyVals[i] = 1;//random(100000);
  }

  Serial.println("Hello Lamp");
  Serial.print("Brightness is set to: ");
  Serial.println(CurrentBri);
  Serial.println("Setup Complete");

  changeScales(5000);
  
  buildPalette();
  for (uint8_t i = 0; i < 4; i++)
  {
    col[i] = pal[i];
  }
  brightnessAreaButton(CurrentBri, 4500, 4000, 4000);
  paletteButton();

  //timeRamp.go(1000000, 100000, LINEAR);
}

// #############################################
// ################## LOOP #####################
void loop() {

  buttonSwitches(); // control button activity
  blendColors( speed1, speed2 );

  makeNoise();

  // Serial.println(random_float(0.5, 1.5)); // Print with 4 decimal places
  // delay(250);

  EVERY_N_MILLISECONDS(200)
  {
    paletteIndex++;
    // Serial.println(timeRamp.update());
  }

  EVERY_N_SECONDS(50)
  {
    coin = random(10);
    if ((coin % 2) == 0)
    {
      //lumRampX.go(random(lumRampX_min, lumRampX_max), 25000, BACK_INOUT);
    }
  }

  EVERY_N_SECONDS(77)
  {
    coin = random(10);
    if ((coin % 2) == 0)
    {
      //lumRampY.go(random(lumRampY_min, lumRampY_max), 35000, BACK_INOUT);
    }
  }

  EVERY_N_SECONDS(20)
  {
      // if (palRamp2.isFinished() == 1 && palette_changed == false)
      // {

      // paletteButton();

      // grant_blend = true;
      // speed1 = 10000;
      // speed2 = 10000;
    //}
  }

  moveRange(lowerRamp.update(), upperRamp.update(), 8);
  //fadeToBlackBy(leds, NUM_LEDS, 128);
  FastLED.show();
  btn.tick();
}

