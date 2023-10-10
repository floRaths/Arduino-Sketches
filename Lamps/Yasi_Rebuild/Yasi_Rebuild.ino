#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

// Pin definitions
#define LED_PIN 5
#define BTN_PIN 7

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

uint8_t switchBrightness = 0;
uint8_t switchArea = 2;

// three brightness values to choose via button
uint8_t Bri1 = 255;
uint8_t Bri2 = 255;
uint8_t Bri3 = 255;

// initial smoothing speeds set (0.5 seconds)
int speed1;
int speed2;

// brightness parameter for smoothe bri changes
uint8_t CurrentBri = 255; // this is the currently running brightness, called by the makenoise function

// prameters for initial palette selection
uint8_t base_hue1 = 0;    // first hue
uint8_t base_hue2 = 170;  // second hue
uint8_t range     = 10;   // fluctuation

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

rampInt noiRamp1; // smooth palette blending 1
rampInt noiRamp2; // smooth palette blending 2

uint8_t hurry = 8;


// #############################################
// ################## SETUP ####################
void setup() {

  delay(250); // startup safety delay
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setBrightness(255); // this brightness will be overridden by makeNoise function

  noiRamp1.go(10000, 10, BACK_INOUT);
  noiRamp1.go(10000, 10, BACK_INOUT);

  Serial.println("Hello Lamp");
  Serial.print("Brightness is set to: ");
  Serial.println(CurrentBri);
  Serial.println("Setup Complete");
  Serial.println("");

  // btn.attachClick(paletteButton);
  }


// #############################################
// ################## LOOP #####################
void loop() {

  startUp(); // startup function... runs only once
  buttonSwitches(); // control button activity
  blendColors( speed1, speed2 );

  makeNoise();

  EVERY_N_SECONDS(2){
      // uint16_t target = random(5, 50) * 1000;
      // Serial.println(target);
      // noiRamp1.go(random(5, 30) * 1000, 25000, BACK_INOUT);
      // noiRamp1.go(random(5, 30) * 1000, 50000, BACK_INOUT);
      //areaButton();
      }

  EVERY_N_SECONDS(30)
  {
      if (palRamp2.isFinished() == 1 && palette_changed == false)
      {

        // // ############# JUST FOR TESTING!!!
        // base_hue1 = random(0, 255);
        // base_hue2 = base_hue1 + random(50, 205);
        // range = random(5, 20);
        // // #############

        grant_blend = true;
        speed1 = 10000;
        speed2 = 22000;

        // Serial.print("Color 1 hue = ");
        // Serial.println(hue[0]);
    }
  }

  moveRange( lowerRamp.update(), upperRamp.update(), 8 );
  fadeToBlackBy(leds, NUM_LEDS, 128);
  
  FastLED.show();
  //btn.tick();
}


// #############################################
// ############## FUNCTIONS ####################

// slowly raises brightness during startup
void startUp()
{
  if (start_up == false) {
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