
#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

// Params for width and height
const uint8_t kMatrixWidth  = 5;
const uint8_t kMatrixHeight = 23;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[ NUM_LEDS ];
#define LAST_VISIBLE_LED 114
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint8_t XYTable[] = {
   110, 111, 112, 113, 114,
   105, 106, 107, 108, 109,
   100, 101, 102, 103, 104,
    95,  96,  97,  98,  99,
    90,  91,  92,  93,  94,
    85,  86,  87,  88,  89,
    80,  81,  82,  83,  84,
    75,  76,  77,  78,  79,
    70,  71,  72,  73,  74,
    65,  66,  67,  68,  69,
    60,  61,  62,  63,  64,
    55,  56,  57,  58,  59,
    50,  51,  52,  53,  54,
    45,  46,  47,  48,  49,
    40,  41,  42,  43,  44,
    35,  36,  37,  38,  39,
    30,  31,  32,  33,  34,
    25,  26,  27,  28,  29,
    20,  21,  22,  23,  24,
    15,  16,  17,  18,  19,
    10,  11,  12,  13,  14,
     5,   6,   7,   8,   9,
     0,   1,   2,   3,   4
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}

uint8_t noiseCols[NUM_LEDS];
uint8_t noiseLuma[kMatrixHeight][kMatrixHeight];

boolean start_up        = false;
boolean new_hues        = false;
boolean palette_changed = false;
boolean grant_blend     = false;
boolean new_colors      = false;

uint8_t switchBrightness = 0;
uint8_t switchArea = 0;

uint8_t Bri1 = 255;
uint8_t Bri2 = 255;
uint8_t Bri3 = 255;

uint8_t CurrentBri = 255;
uint8_t TargetBri  = 255;
uint8_t stored_bri = 255;

uint8_t base_hue1 = 0;
uint8_t base_hue2 = 255;
uint8_t range     = 20;

int lower = 0;
int upper = NUM_LEDS;


int speed1 = 500;
int speed2 = 500;


CRGB    col[4];
CRGB    pal[4];
uint8_t hue[4];
uint8_t sat[4];
uint8_t bri[4];

ramp briRamp;

ramp palRamp1;
ramp palRamp2;

rampInt lowerRamp;
rampInt upperRamp;



void setup() {

  delay(500);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  btn.attachClick(areaButton);

}

void loop() {
  
  startUp();
  buttonSwitches();
  blendColors( speed1, speed2 );

  makeNoise2();

  EVERY_N_SECONDS(3) {
    if ( palRamp2.isFinished() == 1 & palette_changed == false ) {
      grant_blend = true;
      speed1 = 500;
      speed2 = 500;
    }
  }

  moveRange( lowerRamp.update(), upperRamp.update(), 8 );
  fadeToBlackBy(leds, NUM_LEDS, 128);
  
  FastLED.show();
  btn.tick();

}



//   memset(noiseCols, 0, NUM_LEDS);
//   fill_raw_noise8(noiseCols, NUM_LEDS, octaveVal, xVal, scaleVal, timeVal);

//   CRGBPalette16 runPal = CRGBPalette(col[0], col[1], col[2], col[3])

//   for(int x = 0; x < kMatrixWidth; x++) {
//     for(int y = 0; y < kMatrixHeight; y++) {
      
//       leds[XY(x,y)] = 
//       ColorFromPalette( runPal, 
//                         noiseCols[(y * kMatrixWidth) + x],
//                         brighten8_raw(map8(noiseLuma[x, y], 0, CurrentBri))
//                         );

//     }
//   }
// }

void makeNoise2() {

  uint8_t   octaveVal   = 1;
  uint16_t  xVal        = 0;
  int       scaleVal    = 50;
  int       timeVal     = 0;

  timeVal = millis() / 4;

  memset(noiseCols, 0, NUM_LEDS);
  fill_raw_noise8(noiseCols, NUM_LEDS, octaveVal, xVal, scaleVal, timeVal);
  
  CRGBPalette16 runPal = CRGBPalette16(col[0], col[2], col[1], col[3]);

  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(runPal, 
      noiseCols[i], 
      //noiseCols[NUM_LEDS - i - 1]
      map8(noiseCols[NUM_LEDS - i - 1], 0, CurrentBri)
      );
  }
}



void brightnessButton() {
  switchBrightness = (switchBrightness + 1) % 3;
  briRamp.go(TargetBri, 750, CIRCULAR_INOUT);
}



void areaButton() {
  switchArea = (switchArea + 1) % 3;
  lowerRamp.go(lower, 750, CIRCULAR_INOUT);
  upperRamp.go(upper, 750, CIRCULAR_INOUT);
}



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



void blendColors( int ramp1_speed, int ramp2_speed ) {

  if (grant_blend == true) {
    
    buildPalette();
    
    grant_blend = false;
    new_colors  = true;
    
    palRamp1.go(255, ramp1_speed, QUINTIC_IN);
    palRamp2.go(255, ramp2_speed, QUINTIC_IN);

  }

  if ( new_colors == true ) {

    col[0] = nblend(col[0], pal[0], palRamp1.update());
    col[1] = nblend(col[1], pal[1], palRamp2.update());
    col[2] = nblend(col[2], pal[2], palRamp1.update());
    col[3] = nblend(col[3], pal[3], palRamp2.update());

    if (palRamp2.isFinished() == 1) {
      new_colors = false;
      palRamp1.go(0, 0, NONE);
      palRamp2.go(0, 0, NONE);
    }

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



void startUp() {
  if ( start_up == false ) {
    briRamp.go(CurrentBri, 1000, LINEAR);
    start_up = true;
  }
}



void buildPalette() {
  
  hue[0] = random(base_hue1 - range, base_hue1 + range);
  hue[1] = random(base_hue2 - range, base_hue2 + range);
  hue[2] = hue[0] + random(0, 20);
  hue[3] = hue[1] + random(0, 20);

  sat[0] = random(170, 255);
  sat[1] = random(170, 255);
  sat[2] = sat[0] + random(0, 20);
  sat[3] = sat[1] + random(0, 20);

  bri[0] = random(240, 255);
  bri[1] = random(240, 255);
  bri[2] = bri[0] - random(0, 30);
  bri[3] = bri[1] - random(0, 30);

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