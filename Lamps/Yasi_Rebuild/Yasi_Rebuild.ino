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
uint8_t hurry = 10;

// three brightness values to choose via button
uint8_t Bri1 = 255;
uint8_t Bri2 = 75;
uint8_t Bri3 = 75;

// prameters for initial palette selection
uint8_t base_hue1 = 20;  // first hue
uint8_t base_hue2 = 130; // second hue
uint8_t range = 5;       // fluctuation

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

uint8_t paletteIndex;
uint8_t CurrentBri; // this is the currently running brightness, called by the makenoise function
uint8_t coin;
int speed1; // initial smoothing speeds set (0.5 seconds)
int speed2;

uint32_t lumxVal;
uint32_t lumyVal;
uint32_t colxVal;
uint32_t colyVal;

CRGB col[4]; // these are the currently used colors
CRGB pal[4]; // these are colors of a new palette that we blend to

// initializing smoothing functions
ramp briRamp; // brightness smoothing

ramp palRamp1; // smooth palette blending 1
ramp palRamp2; // smooth palette blending 2

rampInt lowerRamp; // smooth area blending 1
rampInt upperRamp; // smooth area blending 2

rampInt lumRampX; // smooth luminance scale blending on X
rampInt lumRampY; // smooth luminance scale blending on Y
rampInt colRampX; // smooth color scale blending on X
rampInt colRampY; // smooth color scale blending on Y

int lumRampX_min = 8000;
int lumRampX_max = 10000;
int lumRampY_min = 8000;
int lumRampY_max = 10000;

int colRampX_min = 5000;
int colRampX_max = 7000;
int colRampY_min = 5000;
int colRampY_max = 7000;

// #############################################
// ################## SETUP ####################
void setup() {

  delay(200); // startup safety delay
  Serial.begin(115200);
  randomSeed(analogRead(0) * 100);

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setBrightness(255); // this brightness will be overridden by makeNoise function

  CurrentBri = Bri1;

  lumxVal = random(100000);
  lumyVal = random(100000);
  colxVal = random(100000);
  colyVal = random(100000);

  lumRampX.go(random(lumRampX_min, lumRampX_max), 10, BACK_INOUT); // when X is low and Y high = banding over height
  lumRampY.go(random(lumRampY_min, lumRampY_max), 10, BACK_INOUT); // when Y is low and X high = banding over width
  colRampX.go(random(colRampX_min, colRampX_max), 10, BACK_INOUT);
  colRampY.go(random(colRampY_min, colRampY_max), 10, BACK_INOUT);

  Serial.println("Hello Lamp");
  Serial.print("Brightness is set to: ");
  Serial.println(CurrentBri);
  Serial.println("Setup Complete");
  Serial.println("");

  btn.attachClick(brightnessAreaButton);
  btn.attachLongPressStart(paletteButton);
  btn.setPressMs(250);
  }


// #############################################
// ################## LOOP #####################
void loop() {

  startUp(); // startup function... runs only once
  buttonSwitches(); // control button activity
  blendColors( speed1, speed2 );
  
  makeNoise();

  EVERY_N_MILLISECONDS(200)
  {
    paletteIndex++;
  }

  EVERY_N_SECONDS(50)
  {
    coin = random(10);
    if ((coin % 2) == 0)
    {
      lumRampX.go(random(lumRampX_min, lumRampX_max), 25000, BACK_INOUT);
    }
  }

  EVERY_N_SECONDS(77)
  {
    coin = random(10);
    if ((coin % 2) == 0)
    {
      lumRampY.go(random(lumRampY_min, lumRampY_max), 35000, BACK_INOUT);
    }
  }

  EVERY_N_SECONDS(30)
  {
      if (palRamp2.isFinished() == 1 && palette_changed == false)
      {
        // ############# JUST FOR TESTING!!!
        base_hue1 = random(0, 255);
        // base_hue2 = base_hue1 + random(50, 205);
        base_hue2 = random(0, 255);
        range = random(5, 20);
        // #############

        grant_blend = true;
        speed1 = 10000;
        speed2 = 22000;
    }
  }

  moveRange(lowerRamp.update(), upperRamp.update(), 8);
  fadeToBlackBy(leds, NUM_LEDS, 128);
  FastLED.show();
  btn.tick();
}


// #############################################
// ############## FUNCTIONS ####################

// slowly raises brightness during startup
void startUp()
{
  if (start_up == false) {
    brightnessAreaButton();
    buildPalette();

    for (uint8_t i = 0; i < 4; i++) {
      col[i] = pal[i];
    }

    briRamp.go(CurrentBri, 1000, LINEAR);
    //lowerRamp.go(lower, 2000, LINEAR);
    //upperRamp.go(upper, 2000, LINEAR);
    start_up = true;
  }
}