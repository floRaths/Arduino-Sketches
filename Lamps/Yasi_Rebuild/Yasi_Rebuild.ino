
#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

OneButton btn = OneButton(BTN_PIN, true, true);

// Params for width and height
const uint8_t kMatrixWidth  = 5;
const uint8_t kMatrixHeight = 23;
boolean       flip          = true;

const uint8_t XYTableSize = kMatrixWidth * kMatrixHeight;
uint8_t       XYTable[XYTableSize];

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[ NUM_LEDS ];

uint8_t LAST_VISIBLE_LED = NUM_LEDS - 1;
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  uint8_t index = 0;
  for (uint8_t y = 0; y < kMatrixHeight; y++) {
    for (uint8_t x = 0; x < kMatrixWidth; x++) {
      
      if ( flip == true ) {
        XYTable[index] = kMatrixWidth * kMatrixHeight - index - 1;
        }
      else {
        XYTable[index] = y * kMatrixWidth + x; 
        };
      
      index++;
    }
  }

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}

uint8_t noiseCols[NUM_LEDS];
uint8_t noiseData[kMatrixHeight][kMatrixHeight];

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


uint32_t timeVal = 0;


void setup() {

  delay(500);
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  btn.attachClick(paletteButton);

}


void loop() {
  
  startUp();
  buttonSwitches();
  blendColors( speed1, speed2 );

  makeNoise();

  EVERY_N_SECONDS(3) {
    if ( palRamp2.isFinished() == 1 && palette_changed == false ) {
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

void makeNoise() {

  // fill_raw_noise16into8   (uint8_t *pData, uint8_t num_points,     uint8_t octaves, uint32_t x, int scalex,                          uint32_t time)
  // fill_raw_2dnoise16into8 (uint8_t *pData, int width, int height,  uint8_t octaves, uint32_t x, int scalex, uint32_t y, int scaley,  uint32_t time)

  timeVal = millis() * 40;

  memset(noiseData, 0, NUM_LEDS);  
  fill_raw_2dnoise16into8 (
    (uint8_t*)noiseData, 
    kMatrixHeight,  //width, 
    kMatrixHeight,  //height, 
    1,              //octaves, 
    5000,           //x, 
    5000,           //scalex, 
    1000,           //y, 
    5000,           //scaley, 
    timeVal
    );

  memset(noiseCols, 0, NUM_LEDS);
  fill_raw_noise16into8(noiseCols, NUM_LEDS, 1, 33000, 550, timeVal);

  CRGBPalette16 runPal = CRGBPalette16(col[0], col[1], col[2], col[3]);

  for(int x = 0; x < kMatrixWidth; x++) {
    for(int y = 0; y < kMatrixHeight; y++) {
      
      leds[XY(x,y)] = 
      ColorFromPalette( runPal, 
                        noiseCols[(y * kMatrixWidth) + x],
                        brighten8_raw(map8(noiseData[x][y], 0, CurrentBri))
                        );

    }
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
    briRamp.go    ( CurrentBri, 1000, LINEAR);
    lowerRamp.go  ( lower,      2000, LINEAR);
    upperRamp.go  ( upper,      2000, LINEAR);
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