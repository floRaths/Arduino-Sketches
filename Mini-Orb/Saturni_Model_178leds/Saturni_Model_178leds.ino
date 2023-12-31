#include <FastLED.h>

#define LED_PIN  5
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS 25

const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 15;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

#define LAST_VISIBLE_LED 178
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }


  const uint8_t XYTable[] = {
   177, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
   176, 224, 175, 223, 174, 222, 173, 221, 172, 220, 171, 219, 170, 218, 169, 217,
   161, 209, 162, 210, 163, 211, 164, 212, 165, 213, 166, 214, 167, 215, 168, 216,
   160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145,
   129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
   128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113,
    97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
    96,  95,  94,  93,  92,  91,  90,  89,  88,  87,  86,  85,  84,  83,  82,  81,
    65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,
    64,  63,  62,  61,  60,  59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,
    33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
    32,  31,  30,  29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,
     9, 201,  10, 202,  11, 203,  12, 204,  13, 205,  14, 206,  15, 207,  16, 208,
     8, 200,   7, 199,   6, 198,   5, 197,   4, 196,   3, 195,   2, 194,   1, 193,
     0, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}




////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//PASTE EFFECT CODE HERE
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}
