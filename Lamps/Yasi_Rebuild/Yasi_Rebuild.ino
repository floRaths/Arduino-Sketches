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

// #############################################
// Params for width and height
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 13;

boolean coil    = true;
boolean flip    = false;
boolean ser_col = true;

#define NUM_LEDS kMatrixWidth *kMatrixHeight
CRGB leds[NUM_LEDS];

// booleans for conditional execution
boolean start_up = false; // allows a small startup sequence
boolean palette_changed = false;
boolean grant_blend = false;
boolean new_colors = false;

uint8_t paletteIndex = 0;

uint8_t switchBrightness;
uint8_t switchArea;

// three brightness values to choose via button
uint8_t Bri1 = 255;
uint8_t Bri2 = 75;
uint8_t Bri3 = 75;

// brightness parameter for smoothe bri changes
uint8_t CurrentBri = Bri1; // this is the currently running brightness, called by the makenoise function

// initial smoothing speeds set (0.5 seconds)
int speed1;
int speed2;

// prameters for initial palette selection
uint8_t base_hue1 = 20;   // first hue
uint8_t base_hue2 = 130;   // second hue
uint8_t range     = 5;   // fluctuation

// parameter for moving the lit area
int lower = 0;        // lower end of lights
int upper = NUM_LEDS; // upper end of lights

CRGB col[4]; // these are the currently used colors
CRGB pal[4]; // these are colors of a new palette that we blend to

// initializing smoothing function
ramp briRamp; // brightness smoothing

ramp palRamp1; // smooth palette blending 1
ramp palRamp2; // smooth palette blending 2

rampInt lowerRamp; // smooth area blending 1
rampInt upperRamp; // smooth area blending 2

rampInt lumRampX; // smooth palette blending 1
rampInt lumRampY; // smooth palette blending 2
rampInt colRampX; // smooth palette blending 1
rampInt colRampY; // smooth palette blending 2

uint8_t hurry = 10;
uint8_t coin;

// #############################################
// ################## SETUP ####################
void setup() {

  delay(2); // startup safety delay
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setBrightness(255); // this brightness will be overridden by makeNoise function

  lumRampX.go(10000, 10, BACK_INOUT);     // when X is low and Y high = banding over height
  lumRampY.go(10000, 10, BACK_INOUT);     // when Y is low and X high = banding over width
  colRampX.go(5000, 10, BACK_INOUT);
  colRampY.go(5000, 10, BACK_INOUT);

  Serial.println("Hello Lamp");
  Serial.print("Brightness is set to: ");
  Serial.println(CurrentBri);
  Serial.println("Setup Complete");
  Serial.println("");

  // btn.attachClick(paletteButton);
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
      lumRampX.go(random16(6500, 30000), 25000, BACK_INOUT);
    }
    // areaButton();
  }

  EVERY_N_SECONDS(77)
  {
    coin = random(10);
    if ((coin % 2) == 0)
    {
      lumRampY.go(random16(6500, 30000), 35000, BACK_INOUT);
    }
    // areaButton();
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

  moveRange(lowerRamp.update(), upperRamp.update(), 8 );
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
    lowerRamp.go(lower, 2000, LINEAR);
    upperRamp.go(upper, 2000, LINEAR);
    start_up = true;
  }
}