#include <quickPatterns.h>

#define CHIPSET     WS2812B
#define DATA_PIN    6
#define NUM_LEDS    89
#define BRIGHTNESS  100
#define COLOR_ORDER GRB         //GRB for WS2812, RGB for WS2811
#define TICKLENGTH  25

// declare array of CRGB objects, used by FastLED
CRGB leds[NUM_LEDS];

//declare quickPatterns controller and pass in main led array
quickPatterns quickPatterns(leds, NUM_LEDS);

void setup() {

  delay(3000); // Recovery delay

  random16_add_entropy(analogRead(0)); // seed random number generator

  // ~ Configure FastLED

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 450);

  FastLED.clear();


  // ~ Configure quickPatterns

  quickPatterns.setTickMillis(TICKLENGTH);

  // Cylon of 8 pixels that cycles through the rainbow (layer 0)
  quickPatterns.layer(0).addPattern(new qpSparkles(100))
  .chooseColorFromPalette(OceanColors_p, SEQUENTIAL)
  .drawEveryNTicks(1);

  //quickPatterns.layer(0).addPattern(new qpConfetti(10))
  //.singleColor(CRGB::Red)
  //.drawEveryNTicks(1);

  
  // Periodically toss in some lightning flashes at random places along the strand, 10 pixels wide (layer 1)
  //quickPatterns.sameScene().addPattern(new qpLightning(10))
  //    .singleColor(CRGB::White)
  //    .activatePeriodicallyEveryNTicks(100, 200) // activate at random intervals between 100 and 200 ticks
  //    .stayActiveForNCycles(2, 4); // each activation will trigger 2 to 4 lightning flashes
}

void loop()
{
  quickPatterns.show();
}
