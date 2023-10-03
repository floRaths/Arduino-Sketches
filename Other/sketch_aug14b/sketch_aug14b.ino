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
      paletteSparkle();
      break;
    case 1:
      fire();
      break;
    case 2:
      blurFade();
      break;
    case 3:
      Sunset1();
      break;  
    case 4:
      upThePole();
      break;
    case 5:
      oneDot();
      break;
    case 6:
      Sunset2();
      break;      
  }

  EVERY_N_SECONDS(30) {
    nextPattern();
  }
  
  FastLED.show();
}

void nextPattern() {
  patternCounter = (patternCounter + 1) % 7;          // Change the number after the % to the number of patterns you have
}




DEFINE_GRADIENT_PALETTE( Conor_PurpleRed_gp ) {
    0,   4,  1, 31,
   31,  55,  1, 16,
   63, 197,  3,  7,
   95,  59,  2, 17,
  127,   6,  2, 34,
  159,  39,  6, 33,
  191, 112, 13, 32,
  223,  56,  9, 35,
  255,  22,  6, 38};



DEFINE_GRADIENT_PALETTE( Conor_PurpleBlue_gp ) {
    0,   1,221, 53,
  159,  39,  6, 33,
  255,  73,  3,178};




//------- Put your patterns below -------//

//----------------------------------------
uint16_t brightnessScale = 500;
uint16_t indexScale = 100;


CRGBPalette16 sparklePal = Conor_PurpleBlue_gp;


void paletteSparkle() {

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t brightness = inoise8(i * brightnessScale, millis() / 1);
    uint8_t index = inoise8(i * indexScale, millis() /10);
    leds[i] = ColorFromPalette(sparklePal, index, brightness);
    //leds[i] = CHSV(0, 255, brightness);
  }
}





//----------------------------------------
CRGBPalette16 firePalette = LavaColors_p;

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
void bounceDot() {

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
void blurFade() {

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
  leds[sinBeat2]  = CRGB::Purple;
  leds[sinBeat3]  = CRGB::Orange;

  EVERY_N_MILLISECONDS(1){
    for(int i = 0; i < 4; i++) {
      blur1d(leds, NUM_LEDS, 50);
    }
  }
  
  fadeToBlackBy(leds, NUM_LEDS, 3);
}




//----------------------------------------
uint8_t hue = 0;

void upThePole() {
    
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


//----------------------------------------
void phaseBeat() {
    
    uint8_t sinBeat   = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
    uint8_t sinBeat2  = beatsin8(30, 0, NUM_LEDS - 1, 0, 85);
    uint8_t sinBeat3  = beatsin8(30, 0, NUM_LEDS - 1, 0, 170);
  
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
void oneDot() {
    
    uint16_t sinBeat = beatsin16( 30 , 0, NUM_LEDS - 1, 0, 0);

    leds[sinBeat] = CRGB::Orange;
    
    fadeToBlackBy(leds, NUM_LEDS, 10);
  
    EVERY_N_MILLISECONDS(10){
      Serial.println(sinBeat);
    }

}




//----------------------------------------

CRGBPalette16 myPal1 = Conor_PurpleRed_gp;

void Sunset1() {

  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal1, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(10){
    paletteIndex++;
  }
}


CRGBPalette16 myPal2 = Conor_PurpleBlue_gp;

void Sunset2() {

  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal2, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(10){
    paletteIndex++;
  }
}
