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

void loop() {

  switch (patternCounter) {
    case 0:
      randomMove();
      break;
    case 1:
      rainbowBeat();
      break;
    case 2:
      redWhiteBlue();
      break;
    case 3:
      Sunset();
      break;
  }

  EVERY_N_SECONDS(5) {
    nextPattern();
  }
  
  FastLED.show();
}

void nextPattern() {
  patternCounter = (patternCounter + 1) % 1;          // Change the number after the % to the number of patterns you have
}






//------- Put your patterns below -------//

//----------------------------------------
void randomMove() {
    
    EVERY_N_MILLISECONDS(75) {
    
    // Create a new HSV color for led[0]
    leds[0] = CHSV(beatsin8(30, 50, 100, 0, 0), random8(), random8(100, 255));
    
    // Copy each pixel to the next one, starting at the far end
    // thereby 'moving' the pattern along the strip
    for (int i = NUM_LEDS - 1; i > 0; i--) {
      leds[i] = leds[i - 1];
    }
  }

}



//----------------------------------------
void oneDot() {
    
    //uint16_t sinBeat = beatsin16( beatsin8(5, 10, 30, 0, 0) , 0, NUM_LEDS - 1, 0, 0);
    uint16_t sinBeat = beatsin16( 30 , 0, NUM_LEDS - 1, 0, 0);

    leds[sinBeat] = CRGB::Blue;
    
    fadeToBlackBy(leds, NUM_LEDS, 10);
  
    EVERY_N_MILLISECONDS(10){
      Serial.println(sinBeat);
    }
}



//----------------------------------------
void phaseBeat() {
    
    uint8_t sinBeat   = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2  = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3  = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);
  
    // If you notice that your pattern is missing out certain LEDs, you
    // will need to use the higher resolution beatsin16 instead. In this
    // case remove the 3 lines above and replace them with the following:
    // uint16_t sinBeat   = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
    // uint16_t sinBeat2  = beatsin16(30, 0, NUM_LEDS - 1, 0, 21845);
    // uint16_t sinBeat3  = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);
  
    leds[sinBeat]   = CRGB::Blue;
    leds[sinBeat2]  = CRGB::Cyan;
    leds[sinBeat3]  = CRGB::White;
    
    fadeToBlackBy(leds, NUM_LEDS, 10);
  
    EVERY_N_MILLISECONDS(10){
      Serial.print(sinBeat);
      Serial.print(",");
      Serial.print(sinBeat2);
      Serial.print(",");
      Serial.println(sinBeat3);
    }
}



//----------------------------------------
uint8_t hue = 0;

void sawtooth() {
    
    uint8_t pos = map(beat16(40, 0), 0, 65535, 0, NUM_LEDS -1);
    leds[pos] = CHSV( hue, 200, 255);
  
    fadeToBlackBy(leds, NUM_LEDS, 3);
  
    EVERY_N_MILLISECONDS( 10 ) {
      hue++;
    }
  
    EVERY_N_MILLISECONDS(10) {
      Serial.println(pos);
    }
}



//---------------------------------------- boring
DEFINE_GRADIENT_PALETTE( browngreen_gp ) { 
    0,    6,  255,    0,     //green
   71,    0,  255,  153,     //bluegreen
  122,  200,  200,  200,     //gray
  181,  110,   61,    6,     //brown
  255,    6,  255,    0      //green
};

CRGBPalette16 orangePink = browngreen_gp;


void gradientBeat() {
  
    uint16_t beatA = beatsin16(30, 0, 255);
    uint16_t beatB = beatsin16(20, 0, 255);
    fill_palette(leds, NUM_LEDS, (beatA + beatB) / 2, 10, orangePink, 255, LINEARBLEND);
}



//----------------------------------------
void blurfade() {

uint8_t sinBeat   = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
  uint8_t sinBeat2  = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
  uint8_t sinBeat3  = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);

  // If you notice that your pattern is missing out certain LEDs, you
  // will need to use the higher resolution beatsin16 instead. In this
  // case remove the 3 lines above and replace them with the following:
  // uint16_t sinBeat   = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  // uint16_t sinBeat2  = beatsin16(30, 0, NUM_LEDS - 1, 0, 21845);
  // uint16_t sinBeat3  = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);

  leds[sinBeat]   = CRGB::Cyan;
  leds[sinBeat2]  = CRGB::Blue;
  leds[sinBeat3]  = CRGB::Red;

  EVERY_N_MILLISECONDS(1){
    for(int i = 0; i < 4; i++) {
      blur1d(leds, NUM_LEDS, 50);
    }
  }
  
  fadeToBlackBy(leds, NUM_LEDS, 3);
}



//----------------------------------------
void movingDots() {

// Waves for LED position
  uint8_t posBeat  = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
  uint8_t posBeat2 = beatsin8(60, 0, NUM_LEDS - 1, 0, 0);
  uint8_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 127);
  uint8_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 127);


  // In the video I use beatsin8 for the positions. For longer strips,
  // the resolution isn't high enough for position and can lead to some
  // LEDs not lighting. If this is the case, use the 16 bit versions below
  // uint16_t posBeat  = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  // uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);
  // uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  // uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for LED color
  uint8_t colBeat  = beatsin8(45, 0, 255, 0, 0);

  leds[(posBeat  + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, 10);
}



//----------------------------------------
CRGBPalette16 palette1 = OceanColors_p;

uint16_t brightnessScale = 150;
uint16_t indexScale = 100;

void twinkle() {

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t brightness = inoise8(i * brightnessScale, millis() / 5);
    uint8_t index = inoise8(i * indexScale, millis() /10);
    leds[i] = ColorFromPalette(palette1, index, brightness);
    //leds[i] = CHSV(0, 255, brightness);
  }
}



//----------------------------------------
CRGBPalette16 firePalette = HeatColors_p;

void fire() {

  int  a = millis();
  
  for (int i = 0; i < NUM_LEDS; i++) {

    // 3D noise, x is constant, we move through time on y and z axis
    // The 60 here will change the scale of the effect, lower is smoother
    // higher is more flickery. The time value for z was in the original code
    // as that was a 2D matrix version. I've left it in here as it looks 
    // nice in 1D too!
    uint8_t noise = inoise8 (0 , i * 60 + a , a / 3);

    // Divides 255 by (NUM_LEDS - 1) and subtracts that amount away from 255 to return 
    // a decreasing number each time e.g. for NUM_LEDS = 18, difference between
    // each point is 15, so:
    // i = 0, math = 255
    // i = 1, math = 240
    // i = 2, math = 225
    // ...
    // i = NUM_LEDS, math =  0
    uint8_t math = abs8(i - (NUM_LEDS-1)) * 255 / (NUM_LEDS-1);

    // Take the noise function from earlier and subtract math from it,
    // so the further along the strip you are, the higher palette index you
    // are likely to get. This results in the base of the fire (white, yellow)
    // being at the far end of the strip
    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette (firePalette, index, 255);    
  }  
}



//----------------------------------------

CRGBPalette16  lavaPalette = CRGBPalette16(
  CRGB::DarkRed,  CRGB::Maroon,   CRGB::DarkRed,  CRGB::Maroon,
  CRGB::DarkRed,  CRGB::Maroon,   CRGB::DarkRed,  CRGB::DarkRed,
  CRGB::DarkRed,  CRGB::DarkRed,  CRGB::Red,      CRGB::Orange,
  CRGB::White,    CRGB::Orange,   CRGB::Red,      CRGB::DarkRed
);


void lava() {

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t brightness = inoise8(i * brightnessScale, millis() / 5);
    uint8_t index = inoise8(i * indexScale, millis() /10);
    leds[i] = ColorFromPalette(lavaPalette, index, brightness);
    //leds[i] = CHSV(0, 255, brightness);
  }
}



//----------------------------------------
void prettyFill() {

  uint8_t octaves = 1;
  uint16_t x = 0;
  int scale = 100;
  uint8_t hue_octaves = 1;
  uint16_t hue_x = 1;
  int hue_scale = 50;
  uint16_t ntime = millis() / 3;
  uint8_t hue_shift = 5;
  
  fill_noise16 (leds, NUM_LEDS, octaves, x, scale, hue_octaves, hue_x, hue_scale, ntime, hue_shift);
}



//----------------------------------------
void movingDots2() {
  
  uint16_t posBeat  = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);

  uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for LED color
  uint8_t colBeat  = beatsin8(45, 0, 255, 0, 0);

  leds[(posBeat + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, 10);
}



//----------------------------------------

// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};


CRGBPalette16 myPal = Sunset_Real_gp;

void Sunset() {

  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(10){
    paletteIndex++;
  }
}



//----------------------------------------
void rainbowBeat() {
  
  uint16_t beatA = beatsin16(30, 0, 255);
  uint16_t beatB = beatsin16(20, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);
}


void redWhiteBlue() {

  uint16_t sinBeat   = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t sinBeat2  = beatsin16(30, 0, NUM_LEDS - 1, 0, 21845);
  uint16_t sinBeat3  = beatsin16(30, 0, NUM_LEDS - 1, 0, 43690);

  leds[sinBeat]   = CRGB::Blue;
  leds[sinBeat2]  = CRGB::Red;
  leds[sinBeat3]  = CRGB::White;
  
  fadeToBlackBy(leds, NUM_LEDS, 10);
}
