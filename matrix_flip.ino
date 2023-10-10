// Params for width and height
const uint8_t kMatrixWidth = 5;
const uint8_t kMatrixHeight = 23;
boolean flip = true;

const uint8_t XYTableSize = kMatrixWidth * kMatrixHeight;
uint8_t XYTable[XYTableSize];

#define NUM_LEDS kMatrixWidth * kMatrixHeight
CRGB leds[NUM_LEDS];

uint8_t LAST_VISIBLE_LED = NUM_LEDS - 1;

uint8_t XY(uint8_t x, uint8_t y)
{
  // any out of bounds address maps to the first hidden pixel
  if ((x >= kMatrixWidth) || (y >= kMatrixHeight))
  {
    return (LAST_VISIBLE_LED + 1);
  }

  uint8_t index = 0;
  for (uint8_t y = 0; y < kMatrixHeight; y++)
  {
    for (uint8_t x = 0; x < kMatrixWidth; x++)
    {

      if (flip == true)
      {
        XYTable[index] = kMatrixWidth * kMatrixHeight - index - 1;
      }
      else
      {
        XYTable[index] = y * kMatrixWidth + x;
      };

      index++;
    }
  }

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}

