#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

// Pin definitions
#define LED_PIN 5
#define BTN_PIN 7

// Button Initialization
OneButton btn = OneButton(BTN_PIN, true, true);


// #############################################
// Params for width and height
const uint8_t kMatrixWidth  = 8;
const uint8_t kMatrixHeight = 13;

boolean coil    = true;
boolean ser_col = true;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

uint8_t XY(uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ((x >= kMatrixWidth) || (y >= kMatrixHeight)) {
    return (NUM_LEDS);
  }

  uint8_t i;
  
  if (coil == true) {
    i = (y * kMatrixWidth) + x;
    }  

  else {
    if (ser_col == true) {
      // Even columns, counting from top to bottom
      if (x % 2 == 0) { 
        i = x * kMatrixHeight + y;
        }
      // Odd columns, counting from bottom to top
      else {
        i = x * kMatrixHeight + (kMatrixHeight - 1 - y);
        }
    }
    // otherwise we operate on rows (Y values)
    else {
      // Even rows, counting from left to right
      if (y % 2 == 0) {
        i = y * kMatrixWidth + x;
        }
      // Odd rows, counting from right to left
      else {
        i = y * kMatrixWidth + (kMatrixWidth - 1 - x);
        }
      }
    }
    return i;
}


// #############################################
// two arrays holding the noise data for colors and luminosity
uint8_t noiseCols[NUM_LEDS];
uint8_t noiseData[kMatrixHeight][kMatrixHeight];

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

// brightness parameter for smoothe bri changes
uint8_t CurrentBri = 75; // this is the currently running brightness, called by the makenoise function
uint8_t TargetBri  = 255;  // when the button is pressed, this is the new brightness to transition to
uint8_t stored_bri = 255; // ???

// prameters for palette selection
uint8_t base_hue1 = 0;   // first hue
uint8_t base_hue2 = 150; // second hue
uint8_t range = 20;      // fluctuation

// parameter for moving the lit area
int lower = 0;        // lower end of lights
int upper = NUM_LEDS; // upper end of lights

// initial smoothing speeds set (0.5 seconds)
int speed1 = 500;
int speed2 = 500;

// arrays for holding colors and their values
CRGB col[4];    // these are the currently used colors
CRGB pal[4];    // these are colors of a new palette that we blend to
uint8_t hue[4]; // used for selectng new palettes
uint8_t sat[4];
uint8_t bri[4];

// initializing smoothing function
ramp briRamp; // brightness smoothing

ramp palRamp1; // smooth palette blending 1
ramp palRamp2; // smooth palette blending 2

rampInt brightnessRamps[NUM_LEDS];

rampInt lowerRamp; // smooth area blending 1
rampInt upperRamp; // smooth area blending 2

uint32_t timeVal;

const unsigned long twinkleDelay = 1000;

CRGBPalette16 myPalette;

// #############################################
// ################## SETUP ####################
void setup() {

  delay(1); // startup safety delay
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setBrightness(100); // this brightness will be overridden by makeNoise function
  
  myPalette = HeatColors_p;
  }

// #############################################
// ################## LOOP #####################
void loop() {

  
  //makeNoise();
  fillMatrixWithPalette();
  twinkle();

  EVERY_N_SECONDS(8)
  {
      if (palRamp2.isFinished() == 1 && palette_changed == false)
      {

        // //############# JUST FOR TESTING!!!
        // base_hue1 = random(0, 255);
        // base_hue2 = base_hue1 + random(50, 205);
        // range = random(5, 20);
        // //#############

        grant_blend = true;
        speed1 = 2000;
        speed2 = 2000;
    }
  }

  
  fadeToBlackBy(leds, NUM_LEDS, 128);
  
  FastLED.show();
  //btn.tick();

}

// #############################################
// ############## FUNCTIONS ####################

void fillMatrixWithPalette()
{
  for (uint8_t y = 0; y < kMatrixHeight; y++)
  {
    for (uint8_t x = 0; x < kMatrixWidth; x++)
    {
        uint8_t index = XY(x, y);

        // Fill the LED at (x, y) with color from the palette
        leds[index] = ColorFromPalette(myPalette, map(x, 0, kMatrixWidth - 1, 0, 150));
    }
  }

  // Show the changes on the LED matrix
  FastLED.show();
}

void twinkle()
{
  // Number of LEDs to twinkle
  const uint8_t numTwinklingLEDs = 5;          // Adjust this to your preference
  const unsigned long twinklingDuration = 200; // Duration in milliseconds

  for (uint8_t i = 0; i < numTwinklingLEDs; i++)
  {
    // Randomly select an LED
    uint8_t x = random(kMatrixWidth);
    uint8_t y = random(kMatrixHeight);
    uint8_t index = XY(x, y);

    // Set the LED to a bright color
    leds[index] = CRGB::White; // You can use any color you like

    // Show the changes on the LED matrix
    FastLED.show();

    // Delay to keep the LED brightened for a short duration
    delay(twinklingDuration);

    // Set the LED back to the original color from the palette
    leds[index] = ColorFromPalette(myPalette, map(x, 0, kMatrixWidth - 1, 0, 255));
  }
}
