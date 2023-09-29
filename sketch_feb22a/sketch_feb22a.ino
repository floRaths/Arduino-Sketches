#include <Arduino.h>
#define FASTLED_INTERNAL
#include "FastLED.h"


// How many leds in your strip?
#define NUM_LEDS 89
#define DATA_PIN 6

CRGB g_LEDs[NUM_LEDS] = {0};
int g_Brightness = 255;

#define NUM_COLORS 5
static const CRGB TwinkleColors [NUM_COLORS] = 
{
    CRGB::Red,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Green,
    CRGB::Yellow
};

void DrawTwinkle()
{
    FastLED.clear(false);

    for (int i=0; i<NUM_LEDS/4; i++) 
    {
        g_LEDs[random(NUM_LEDS)] = TwinkleColors[random(0, NUM_COLORS)];
        FastLED.show(g_Brightness);
        delay(200);
    }
}
