/* Hello Wokwi! */
#include "FastLED.h"

const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 13;
boolean coil = true;
boolean flip = false;
boolean ser_col = true;
uint8_t hurry = 5;
boolean dataSmoothing = true;

#define NUM_LEDS kMatrixWidth *kMatrixHeight
CRGB leds[NUM_LEDS];

uint8_t lumNoise[kMatrixHeight][kMatrixHeight];
uint8_t prevlumNoise[kMatrixHeight][kMatrixHeight];

int currentMin = 50; // Initialize currentMin to the maximum possible integer value
int currentMax = 50; // Initialize currentMax to the minimum possible integer value

int maxValue = 0; // Assume the first element is the maximum
 
void setup() {
  Serial.begin(115200); 
}

void loop()
{
  EVERY_N_MILLISECONDS(100){
      // Print the current minimum and maximum values
      // printMinMax();
      // lumNoise[4][10] = lumNoise[4][10] + 1;
      // Serial.println(contrast);
  }
  
  makeNoise();

  //printBand(4);
}

void printBand(uint8_t col){
for (int row = 0; row < kMatrixHeight; row++)
{
  Serial.print(lumNoise[col][row]);
  Serial.print(",");
}
Serial.println();
}

void printMinMax()
{
for (int x = 0; x < kMatrixWidth; x++)
{
  for (int y = 0; y < kMatrixHeight; y++)
  {
    int newValue = lumNoise[x][y];

    // Update the current minimum and maximum values
    if (newValue < currentMin)
    {
      currentMin = newValue;
    }
    if (newValue > currentMax)
    {
      currentMax = newValue;
    }
  }
}
// Print the current minimum and maximum values
Serial.print("Current Minimum: ");
Serial.println(currentMin);
Serial.print("Current Maximum: ");
Serial.println(currentMax);
// Serial.print("Contrast: ");
// Serial.println(contrast);
// Serial.println(leds[mtx(col, row)].getLuma());
}


// // Iterate through the array to find the maximum value
// for (int i = 1; i < arrayLength; i++)
// {
// if (array[i] > maxValue)
// {
//   maxValue = array[i]; // Update the maximum value if a larger value is found
// }
// }

//   EVERY_N_MILLISECONDS(100)
//   {
//     // if (maxValue == 255)
//     // {
//     //   contrast--;
//     // }
//     // else
//       contrast++;
//   }
// }
