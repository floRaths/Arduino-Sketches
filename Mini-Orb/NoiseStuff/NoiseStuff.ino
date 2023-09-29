#include <FastLED.h>

#define NUM_LEDS 110
#define LED_PIN 5

CRGB      leds[NUM_LEDS];
uint8_t   noiseData[NUM_LEDS];


DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {

    0, 255, 20,   0,   // red
    //0, 0,  0,   0,   // red
   43, 255, 70, 22,   // red orange
   86, 255, 85, 0,   // gol orange
  125, 183, 38,  13,  // red2
  172, 100, 0,   103, // light purple
  215, 75,  0,   130, // indigo
  255, 118, 0,   118  // purple
  };


CRGBPalette16 party = Sunset_Real_gp;



uint8_t   octaveVal   = 1;
//uint16_t  xVal        = 10;
int       scaleVal    = 5;
uint32_t  timeVal     = 1000;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  Serial.begin(115200);
}


void loop() {

  //int scaleVal       = beatsin88(0.04, 1, 5, 0, 0);
  //uint32_t xVal      = beatsin88(0.05, 1, 500, 0, 0);

  //int scaleVal       = beatsin88(10, 1, 5, 0, 0);
  uint32_t xVal      = beatsin16(0.1,  10000, 30000, 0, 0);


  timeVal = millis() / 18;

  memset(noiseData, 0, NUM_LEDS);
  fill_raw_noise8(noiseData, NUM_LEDS, octaveVal, xVal, scaleVal, timeVal);
  
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(party, noiseData[i], noiseData[NUM_LEDS - i - 1]-20);
      
  }
  
  FastLED.show();
}
