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
const uint8_t kMatrixWidth  = 10;
const uint8_t kMatrixHeight = 11;

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
boolean new_hues = false; //
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
uint8_t CurrentBri = 100; // this is the currently running brightness, called by the makenoise function
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

rampInt lowerRamp; // smooth area blending 1
rampInt upperRamp; // smooth area blending 2

uint32_t timeVal;


// #############################################
// ################## SETUP ####################
void setup() {

  delay(1); // startup safety delay
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  FastLED.setBrightness(255); // this brightness will be overridden by makeNoise function

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

  EVERY_N_SECONDS(8) {
    if ( palRamp2.isFinished() == 1 && palette_changed == false ) {

        //############# JUST FOR TESTING!!!
        base_hue1 = random(0, 255);
        base_hue2 = base_hue1 + random(50, 205);
        range = random(5, 20);
        //#############

        grant_blend = true;
        speed1 = 2000;
        speed2 = 2000;
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


void makeNoise() {

  timeVal = millis() * 25;

  memset(noiseData, 0, NUM_LEDS);  
  fill_raw_2dnoise16into8 (
    (uint8_t*)noiseData,
    kMatrixHeight,  // width 
    kMatrixHeight,  // height 
    1,              // octaves 
    5000,           // x
    5000,           // scalex 
    1000,           // y
    5000,           // scaley
    timeVal         // timeVal
    );

  memset(noiseCols, 0, NUM_LEDS);
  fill_raw_noise16into8(noiseCols, NUM_LEDS, 1, 33000, 550, timeVal);

  CRGBPalette16 runPal = CRGBPalette16(col[0], col[1], col[2], col[3]);

  for(int x = 0; x < kMatrixWidth; x++) {
    for(int y = 0; y < kMatrixHeight; y++) {
      
      leds[XY(x,y)] = 
      ColorFromPalette(runPal, 
                       noiseCols[(y * kMatrixWidth) + x],
                       brighten8_raw(map8(noiseData[x][y], 0, CurrentBri))
                       );

    }
  }

  // fill_raw_noise16into8   (uint8_t *pData, uint8_t num_points,     uint8_t octaves, uint32_t x, int scalex,                          uint32_t time)
  // fill_raw_2dnoise16into8 (uint8_t *pData, int width, int height,  uint8_t octaves, uint32_t x, int scalex, uint32_t y, int scaley,  uint32_t time)

}


// Activates smooth blending to new brightness
void brightnessButton() {
  switchBrightness = (switchBrightness + 1) % 3;
  briRamp.go(TargetBri, 750, CIRCULAR_INOUT);
}

// Activates smooth blending to new area
void areaButton() {
  switchArea = (switchArea + 1) % 3;
  lowerRamp.go(lower, 750, CIRCULAR_INOUT);
  upperRamp.go(upper, 750, CIRCULAR_INOUT);
}

// Picks new colors, then triggers color blending and goes dark, 
// to signal that the palette has been changed
void paletteButton() {
  
  //if (new_hues == false) {
    base_hue1 = random(0, 255);
    base_hue2 = base_hue1 + random(50, 205);
    range = random(5, 20);
    //new_hues = true;
  //}
  
  palette_changed = true;
  grant_blend = true;
  speed1 = 2000;
  speed2 = 2000;
  stored_bri = CurrentBri;
  briRamp.go(0, 50, LINEAR);
}


// when a new palette is requested, the old colors are blended towards that new state
void blendColors( int ramp1_speed, int ramp2_speed ) {

  // grant_blend is false until paletteButton is pressed or after x seconds have passed
  if (grant_blend == true)
  {

    // creating a new palette
    buildPalette();

    grant_blend = false; // deactivate grant_blend
    new_colors = true;   // allows blending of old colors to new colors

    // start smoothly blending colors
    palRamp1.go(255, ramp1_speed, QUINTIC_IN);
    palRamp2.go(255, ramp2_speed, QUINTIC_IN);
  }

  if ( new_colors == true ) {

    // with above smoothing active, the colors are blended to the new palette
    col[0] = nblend(col[0], pal[0], palRamp1.update());
    col[1] = nblend(col[1], pal[1], palRamp2.update());
    col[2] = nblend(col[2], pal[2], palRamp1.update());
    col[3] = nblend(col[3], pal[3], palRamp2.update());

    // after the slower ramp is done, we terminate the color blending and re-set the used ramps
    if (palRamp2.isFinished() == 1) {
      new_colors = false;
      palRamp1.go(0, 0, NONE);
      palRamp2.go(0, 0, NONE);
    }

    // ????
    if (palRamp2.isFinished() == 1 & palette_changed == true) {
      palette_changed == false;
      briRamp.go(stored_bri, 500, LINEAR);
    }
  }
}

void moveRange( uint8_t lower, uint8_t upper, uint8_t steps ) {  
  
  for(int i = upper; i < NUM_LEDS; i++) {

    int value = (255/steps) * (i - upper);
    if (value >= 255) value = 255;
    leds[i].subtractFromRGB(value);
  }

  for(int k = lower; k > -1; k--) {

    int value = (255/steps) * (lower - k);
    if (value >= 255) value = 255;
    leds[k].subtractFromRGB(value);
  }
}


// randomly creates a new 4-color palette based on two main hues
// the general logic of random selection is used for hue, sat, bri for each color
void buildPalette() {

  hue[0] = random(base_hue1 - range, base_hue1 + range);  // pick hue of first color with allowance of 'range'
  hue[1] = random(base_hue2 - range, base_hue2 + range);  // pick hue of second color with allowance of 'range'
  hue[2] = hue[0] + random(0, 20);                        // pick hue of third color, which can be up to 20-clicks different from first
  hue[3] = hue[1] + random(0, 20);                        // pick hue of fourth color, which can be up to 20-clicks different from second

  sat[0] = random(170, 255);                              // pick saturation for first color
  sat[1] = random(170, 255);                              // pick saturation for second color
  sat[2] = sat[0] + random(0, 20);                        // same logic as for hues above
  sat[3] = sat[1] + random(0, 20);                        // same logic as for hues above

  bri[0] = random(240, 255);                              // same logic as for hues and sat above
  bri[1] = random(240, 255);
  bri[2] = bri[0] - random(0, 30);
  bri[3] = bri[1] - random(0, 30);

  // after selecting our 12 color components, we store the results in the palette
  for (uint8_t i = 0; i < 4; i++) {
    pal[i] = CHSV(hue[i], sat[i], bri[i]);
  }

}


void buttonSwitches() {
  
  CurrentBri = briRamp.update();
  switch (switchBrightness) {
    case 0:
      TargetBri = Bri1;
      break;
    case 1:
      TargetBri = Bri2;
      break;
    case 2:
      TargetBri = Bri3;
      break;
  }

  switch (switchArea) {
    case 0:
      lower = 0;
      upper = NUM_LEDS;
      break;
    case 1:
      lower = 30;
      upper = 60;
      break;
    case 2:
      lower = 50;
      upper = 70;
      break;
  }
}