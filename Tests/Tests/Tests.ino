#include <FastLED.h>

#define NUM_LEDS 72
#define LED_PIN 5

CRGB      leds[NUM_LEDS];
uint8_t   noiseData[NUM_LEDS];


DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
    0, 255, 0,   0,   // red
   43, 255, 79, 22,  // red orange
   86, 255, 104, 0,   // gol orange
  125, 183, 38,  13,  // red2
  172, 100, 0,   103, // light purple
  215, 75,  0,   130, // indigo
  255, 118, 0,   118  // purple
  };



//CRGBPalette16 party = Sunset_Real_gp;


uint8_t   octaveVal   = 1;
//uint16_t  xVal        = 10;
//int       scaleVal    = 5;
uint16_t  timeVal     = 10;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  Serial.begin(115200);
}


CRGBPalette16 myPal = Sunset_Real_gp;
uint8_t paletteIndex = 0;

void loop() {

  fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
  
  EVERY_N_MILLISECONDS(10){
    paletteIndex++;
  }
  
  FastLED.show();
}
