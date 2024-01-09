#include <FastLED.h>

uint8_t colA, colB, colC, colD;

// ################## SETUP ####################
void setup()
{
  Serial.begin(115200);
}


void loop()
{
  EVERY_N_MILLISECONDS(500)
  {
    generateNewHues(colA, colB, colC, colD, 30);

    Serial.println("generated");
    Serial.println(colA);
    // Serial.println(colB);
    // Serial.println(colC);
    // Serial.println(colD);
  }
}

// Helper function to test if two hues are separate by a desired distance
bool testDistance(uint8_t num1, uint8_t num2, uint8_t minDistance)
{
  uint8_t test1 = num1 - num2;
  uint8_t test2 = num2 - num1;
  return (test1 <= minDistance) || (test2 <= minDistance);
}


// Function to generate three random values with a defined minimum separation
void generateNewHues(uint8_t &hue_A, uint8_t &hue_B, uint8_t &hue_C, uint8_t &hue_D, const uint8_t minDistance)
{
  uint8_t hue_old = hue_A;

  do // Generate the first color
  {
    hue_A = random(256);
  } while (testDistance(hue_A, hue_old, minDistance));

  do // Generate the second color
  {
    hue_B = random(256);
  } while (testDistance(hue_B, hue_A, minDistance));

  do // Generate the third color
  {
    hue_C = random(256);
  } while (testDistance(hue_C, hue_A, minDistance) || testDistance(hue_C, hue_B, minDistance));

  do // Generate the fourth color
  {
    hue_D = random(256);
  } while (testDistance(hue_D, hue_A, minDistance) || testDistance(hue_D, hue_B, minDistance) || testDistance(hue_D, hue_C, minDistance));
}