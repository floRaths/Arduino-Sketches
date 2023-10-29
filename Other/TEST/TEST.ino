// /***********************************/
// #include <FastLED.h>

// #define NUM_LEDS 102
// #define COLOR_ORDER GRB
// #define LED_TYPE WS2812B
// #define MAX_BRIGHTNESS 255

// struct CRGB leds[NUM_LEDS];

// void setup()
// {
//     delay(500);
//     LEDS.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
//     FastLED.setBrightness(MAX_BRIGHTNESS);
//     fill_solid(leds, NUM_LEDS, CRGB(255, 255, 255));
// }

// void loop()
// {
//     drawBreathe();
//     // FastLED.show();
// }

// void drawBreathe()
// {

//     float in, breath;

//     for (in = 0; in < 6.283; in = in + 0.001)
//     {
//         breath = sin(in) * 127.5 + 127.5;
//         fill_solid(leds, NUM_LEDS, CRGB(breath, breath, breath));
//         FastLED.show();
//     }
// }
/***********************************/

// //and the exp(sin(mills()) code:
// /***********************************/
#include <FastLED.h>

#define NUM_LEDS 102
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define MAX_BRIGHTNESS 255

struct CRGB leds[NUM_LEDS];

void setup()
{
    delay(1000);
    LEDS.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(MAX_BRIGHTNESS);
    FastLED.clear();
    fill_solid(leds, NUM_LEDS, CRGB::White);
}

void loop () {
    float breath = (exp(sin(millis() / 7000.0 * PI)) - 0.36787944)108.0; // was (exp(sin(millis()/2000.0PI)) - 0.36787944)*108.0
    FastLED.setBrightness(breath);
    FastLED.show();
}

/***********************************/