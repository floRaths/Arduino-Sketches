#include <FastLED.h>

#define NUM_LEDS  150
#define LED_PIN   5

CRGB leds[NUM_LEDS];

uint8_t patternCounter = 0;
uint8_t paletteIndex = 0;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  Serial.begin(57600);
}


DEFINE_GRADIENT_PALETTE( bhw1_28_gp ) {
    0,   0,  0, 0,
    0,   0,  0, 0,
    0,  75,  1,221,
   30, 252, 73,255,
   48, 169,  0,242,
  119,   0,149,242,
  170,  43,  0,242,
  206, 252, 73,255,
  232,  78, 12,214,
  255,   0,149,242};

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};

DEFINE_GRADIENT_PALETTE( Conor_PurpleBlue_gp ) {
    0,   1,221, 53,
  159,  39,  6, 33,
  255,  73,  3,178};

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp_black ) {
    0,   0,  0,  0,
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160}; 

  

void loop() {
  switch (patternCounter) {
    case 0:
      Sunset1();
      break;
    case 1:
      BlurFade();
      break;  
    case 2:
      Sparkle();
      break;
    case 3:
      SunsetFire();
      break;  
    case 4:
      upThePole();
      break;
    case 5:
      Sunset2();
      break;
    case 6:
      BounceDot();
      break;
    case 7:
      TheNotch();
      break;
    case 8:
      UpDown();
      break;      
    case 9:
      PurpSpark();
      break;              
  }

  EVERY_N_SECONDS(32) {
    nextPattern();
  }
  
  FastLED.show();
}


void nextPattern() {
  patternCounter = (patternCounter + 1) % 10;          // Change the number after the % to the number of patterns you have
}




//------- Put your patterns below -------//

CRGBPalette16 myPal = Sunset_Real_gp;

void Sunset1() {

  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(10){
    paletteIndex++;
  }
}


//----------------------------------------
uint8_t gHue = 0; 

void Sparkle() {
  
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10 );
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(14), 0, 255 );

}


//----------------------------------------
uint8_t hue = 0;

void upThePole() {
    
    uint8_t pos = map(beat16(40, 0), 0, 65535, 0, NUM_LEDS -1);
    leds[pos] = CHSV( hue, 200, 255);
  
    fadeToBlackBy(leds, NUM_LEDS, 3);
  
    EVERY_N_MILLISECONDS( 5 ) {
      hue++;
    }
  
    EVERY_N_MILLISECONDS(10) {
      Serial.println(pos);
    }
}


//----------------------------------------
void BlurFade() {

  uint8_t sinBeat   = beatsin8(8, 0, NUM_LEDS - 1, 0, 0);
  uint8_t sinBeat2  = beatsin8(5, 0, NUM_LEDS - 1, 0, 85);
  uint8_t sinBeat3  = beatsin8(10, 0, NUM_LEDS - 1, 0, 170);

  leds[sinBeat]   = CRGB::Purple;
  leds[sinBeat2]  = CRGB::DarkCyan;
  leds[sinBeat3]  = CRGB::Orange;

  EVERY_N_MILLISECONDS(1){
    for(int i = 0; i < 4; i++) {
      blur1d(leds, NUM_LEDS, 50);
    }
  }
  
  fadeToBlackBy(leds, NUM_LEDS, 3);
}


//----------------------------------------
CRGBPalette16 myPal1 = Conor_PurpleBlue_gp;

void Sunset2() {

  fill_palette(leds, NUM_LEDS, -paletteIndex, 255 / NUM_LEDS, myPal1, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(10){
    paletteIndex++;
  }
}


//----------------------------------------
CRGBPalette16 firePalette = Sunset_Real_gp_black;

void SunsetFire() {

  int  a = millis();
  
  for (int i = 0; i < NUM_LEDS; i++) {

    uint8_t noise = inoise8 (0 , i * 60 + a , a / 3);
    uint8_t math = abs8(i - (NUM_LEDS-1)) * 255 / (NUM_LEDS-1);

    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette (firePalette, index, 255);    
  }  
}



//----------------------------------------
void BounceDot() {

  // Waves for LED position
  uint16_t posBeat  = beatsin16(10, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat2 = beatsin16(20, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat3 = beatsin16(25, 0, NUM_LEDS - 1, 0, 32767);
  uint16_t posBeat4 = beatsin16(12, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for LED color
  uint8_t colBeat  = beatsin8(45, 135, 225, 0, 0);

  // Wave for Fade 
    uint8_t fadeBeat  = beatsin8(18, 0, 20, 0, 0);

  leds[(posBeat  + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, fadeBeat);
}


//----------------------------------------
void UpDown() {
    
    uint16_t sinBeat = beatsin16( 9 , 0, NUM_LEDS - 1, 0, 0);

    // Wave for LED color
    uint8_t colBeat  = beatsin8(20, 135, 225, 0, 0);

    // Wave for Fade 
    uint8_t fadeBeat  = beatsin8(18, 0, 20, 0, 0);
    
    leds[sinBeat] = CHSV(colBeat, 255, 255);
    
    fadeToBlackBy(leds, NUM_LEDS, fadeBeat);
  
    EVERY_N_MILLISECONDS(10){
      Serial.println(sinBeat);
    }
}


//----------------------------------------
uint8_t Hue = 180; 

void PurpSpark() {

  // Wave for FadeFade 
  uint8_t fade  = beatsin8(4, 5, 15, 0, 0);

  // Wave for Fade 
  uint8_t fadeBeat  = beatsin8(fade, 1, 10, 0, 0);
  
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, fadeBeat );
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( Hue + random8(14), 80, 255 );

}


//----------------------------------------
CRGBPalette16 myPal2 = bhw1_28_gp;

void TheNotch() {

  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal2, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(15){
    paletteIndex++;
  }
}
