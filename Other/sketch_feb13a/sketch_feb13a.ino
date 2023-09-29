#include <FastLED.h>

#define LED_PIN  5
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS 25

const uint8_t kMatrixWidth = 9;
const uint8_t kMatrixHeight = 11;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

#define LAST_VISIBLE_LED 88
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint8_t XYTable[] = {
    98,  81,  82,  83,  84,  85,  86,  87,  88,
    97,  80,  79,  78,  77,  76,  75,  74,  73,
    96,  65,  66,  67,  68,  69,  70,  71,  72,
    95,  64,  63,  62,  61,  60,  59,  58,  57,
    94,  49,  50,  51,  52,  53,  54,  55,  56,
    93,  48,  47,  46,  45,  44,  43,  42,  41,
    92,  33,  34,  35,  36,  37,  38,  39,  40,
    91,  32,  31,  30,  29,  28,  27,  26,  25,
    90,  17,  18,  19,  20,  21,  22,  23,  24,
    89,  16,  15,  14,  13,  12,  11,  10,   9,
    0,   1,   2,   3,   4,   5,   6,   7,   8
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}





uint8_t CentreX =  (kMatrixWidth  / 2) - 1;
uint8_t CentreY =  (kMatrixHeight / 2) - 1;



DEFINE_GRADIENT_PALETTE( pit ) {
  0,     3,   3,   3,
  64,   13,   13, 255,  //blue
  128,   3,   3,   3,
  192, 255, 130,   3 ,  //orange
  255,   3,   3,   3
};



// parameters and buffer for the noise array
#define NUM_LAYERS 1
uint32_t x[NUM_LAYERS];
uint32_t y[NUM_LAYERS];
uint32_t z[NUM_LAYERS];
uint32_t scale_x[NUM_LAYERS];
uint32_t scale_y[NUM_LAYERS];
uint8_t  noise[1][16][16];

void loop() {

  noise_noise1();

  // check the Serial Monitor to see how many fps you get
  EVERY_N_MILLIS(1000) {
    Serial.println(LEDS.getFPS());
  }
}



// cheap correction with gamma 2.0
void adjust_gamma()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i].r = dim8_video(leds[i].r);
    leds[i].g = dim8_video(leds[i].g);
    leds[i].b = dim8_video(leds[i].b);
  }
}

//as shown on youtube
//a noise controlled & modulated by itself
void noise_noise1() {

  CRGBPalette16 Pal( pit );

  //modulate the position so that it increases/decreases x
  //(here based on the top left pixel - it could be any position else)
  //the factor "2" defines the max speed of the x movement
  //the "-255" defines the median moving direction
  x[0] = x[0] + (2 * noise[0][0][0]) - 255;
  //modulate the position so that it increases/decreases y
  //(here based on the top right pixel - it could be any position else)
  y[0] = y[0] + (2 * noise[0][kMatrixWidth-1][0]) - 255;
  //z just in one direction but with the additional "1" to make sure to never get stuck
  //in case the movement is stopped by a crazy parameter (noise data) combination
  //(here based on the down left pixel - it could be any position else)
  z[0] += 1 + ((noise[0][0][ kMatrixHeight-1]) / 4);
  //set the scaling based on left and right pixel of the middle line
  //here you can set the range of the zoom in both dimensions
  scale_x[0] = 8000 + (noise[0][0][CentreY] * 16);
  scale_y[0] = 8000 + (noise[0][kMatrixWidth-1][CentreY] * 16);

  //calculate the noise data
  uint8_t layer = 0;
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    uint32_t ioffset = scale_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j <  kMatrixHeight; j++) {
      uint32_t joffset = scale_y[layer] * (j - CentreY);
      uint16_t data = inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer]);
      // limit the 16 bit results to the interesting range
      if (data < 11000) data = 11000;
      if (data > 51000) data = 51000;
      // normalize
      data = data - 11000;
      // scale down that the result fits into a byte
      data = data / 161;
      // store the result in the array
      noise[layer][i][j] = data;
    }
  }

  //map the colors
  for (uint8_t y = 0; y <  kMatrixHeight; y++) {
    for (uint8_t x = 0; x < kMatrixWidth; x++) {
      //I will add this overlay CRGB later for more colors
      //it´s basically a rainbow mapping with an inverted brightness mask
      CRGB overlay = CHSV(noise[0][y][x], 255, noise[0][x][y]);
      //here the actual colormapping happens - note the additional colorshift caused by the down right pixel noise[0][15][15]
      leds[XY(x, y)] = ColorFromPalette( Pal, noise[0][kMatrixWidth-1][ kMatrixHeight-1] + noise[0][x][y]) + overlay;
    }
  }

  //make it looking nice
  adjust_gamma();

  //and show it!
  FastLED.show();
}


void setup() {

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}
