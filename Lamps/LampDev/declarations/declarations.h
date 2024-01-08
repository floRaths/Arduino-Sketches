// ################## inits ###################
// booleans for conditional execution
boolean palette_changed = false;
boolean new_colors = false;
boolean pressed = false;
boolean rainbow = false;
boolean rolling = false;
boolean triple = false;
boolean indexDrift = false;

uint8_t paletteIndex, CurrentBri, TargetBri, coin;
int speed1, speed2; // initial smoothing speeds set (0.5 seconds)
uint32_t xyVals[4];
int noiRampMin[4];
int noiRampMax[4];
int rampStore[4];

CRGB runCol[4];  // these are the currently used colors
CRGB newCol[4];  // these are the currently used colors
CRGB newCol2[4]; // these are the currently used colors
CRGB oldCol[4];  // these are the currently used colors
CRGB tmpCol[4];  // these are the currently used colors

// parameter for moving the lit area
uint16_t lower = 0;            // lower end of lights
uint16_t upper = NUM_LEDS - 4; // upper end of lights
uint16_t store, up_speed, lo_speed, bri_speed;

// initializing smoothing functions
rampInt briRamp, palRamp1, palRamp2;             // smooth palette blending 1
ramp lowerRamp, upperRamp;                       // smooth area blending 1
rampLong lumRampX, lumRampY, colRampX, colRampY; // smooth luminance scale blending on X

// arrays holding the raw, intermediate and final noise data for colors and luminosity
uint8_t lumNoise[kMatrixHeight][kMatrixHeight];
uint8_t adj_lumData[kMatrixHeight][kMatrixHeight];
uint8_t scaled_lumData[kMatrixHeight][kMatrixHeight];

uint8_t colNoise[kMatrixHeight][kMatrixHeight];
uint8_t adj_colData[kMatrixHeight][kMatrixHeight];
//uint8_t scaled_colData[kMatrixHeight][kMatrixHeight];

int maxLumValue = 0; // Assume the first element is the maximum

uint8_t list[] = {0, 1, 2, 3};

uint8_t switchBrightness = 0;
uint8_t switchArea = 2;
uint8_t switchIndex = 4;

uint8_t stored_bri; // when the button is pressed, this is the new brightness to transition to