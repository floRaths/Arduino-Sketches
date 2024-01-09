#include <FastLED.h>
#include "auxFnss/auxFnss.h"

uint8_t hue_A = 10;
uint8_t hue_B = 10;
uint8_t hue_C = 10;
uint8_t hue_D = 10;

uint8_t colorScheme;

CRGB newCol[4];

colorProperty color[4];

void setup() {}

void loop() {

    switch (colorScheme)
    {
    case 0: // monochrome
        color[0] = {base_hue1, 10, 255, 0, 255};
        color[1] = {base_hue1, 10, 255, 0, 255};
        color[2] = {base_hue1, 10, 255, 0, 255};
        color[3] = {base_hue1, 10, 255, 0, 255};
        break;

    case 1: // duotone
        color[0] = {base_hue1, 10, 255, 0, 255};
        color[1] = {base_hue1, 10, 255, 0, 255};
        color[2] = {base_hue2, 10, 255, 0, 255};
        color[3] = {base_hue2, 10, 255, 0, 255};
        break;
    }

}