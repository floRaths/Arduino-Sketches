#include <FastLED.h>
#include "auxFnss/auxFnss.h"

void setup() {}

void loop() {

    // uint8_t satRange[2] = {50, 100}; // Example saturation range
    // uint8_t briRange[2] = {20, 80};  // Example brightness range

    Range satRange = {50, 100}; // Example saturation range
    Range briRange = {20, 80};  // Example brightness range

    CHSV myColor = makeColors(120, 10, satRange, briRange);
    

    //fill_raw_2dnoise8();
}