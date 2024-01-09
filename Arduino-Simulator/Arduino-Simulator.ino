#include <FastLED.h>
#include "auxFnss/auxFnss.h"


uint8_t hueA, hueB, hueC, hueD;

// ################## SETUP ####################
void setup()
{
  Serial.begin(115200);
  
  // initial palette instruction
  color[0] = {20, 10, 255, 255, 255, 255};
  color[1] = {20, 10, 255, 255, 255, 255};
  color[2] = {20, 10, 255, 255, 255, 255};
  color[3] = {20, 10, 255, 255, 255, 255};
}


void loop()
{
  EVERY_N_MILLISECONDS(500)
  {
    generateNewHues(hueA, hueB, hueC, hueD, 30, true);

    // this is a monochrome palette instruction
    color[0] = {hueA, 0, 255, 255, 255, 255};
    color[1] = {hueA, 0, 255, 255, 255, 255};
    color[2] = {hueA, 0, 255, 255, 255, 255};
    color[3] = {hueA, 0, 255, 255, 255, 255};

    assemblePalette(true, true);
  }
}
