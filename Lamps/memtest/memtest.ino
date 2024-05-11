#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>
//#include <Adafruit_DotStar.h>

//Adafruit_DotStar strip(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);

// Pin definitions
#define LED_PIN 5
#define BTN_PIN 7

// Button Initialization
OneButton btn = OneButton(BTN_PIN, true, true);

#define NUM_LEDS 120
CRGB leds[NUM_LEDS];

void setup()
{
    // put your setup code here, to run once:
    // Serial.begin(9600);
    delay(100); // startup safety delay
    Serial.begin(9600);
    randomSeed(analogRead(0));

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setTemperature(Tungsten40W);
    FastLED.setBrightness(0);

    // btn.attachClick(buttonClick);
    // btn.attachLongPressStart(buttonHold);
    btn.setPressMs(250);

    Serial.println();
    Serial.println("######## Hello Lamp ########");
    Serial.println("############################");

    // pllt.hueA = 30;
    // pllt.hueB = 50;
    // // pllt.hueC = 170;
    // pllt.paletteType = "duotone";

    // scls.colScales = {2500, 15000, 2500, 15000};
    // scls.lumScales = {5000, 18000, 5000, 18000};

    // strp.upper_limit = LAST_LED - 4;
    // strp.lower_limit = 0;

    // initializePerlin(scls, 500, 10000);

    // // generateNewHues (pllt, 30, true, true);
    // updatePalette(pllt, pllt.paletteType, false, true);
    // changeScales(scls, 15000, true, true);
    // changeBrightness(3500, false, brightnessVals[0], true);
    // changeStripRange(strp, false, true, strp.upper_limit, strp.lower_limit, 5000);

    // triggerBlend(pllt, 50, true, true);
    // blendColors(pllt);
}

void loop()
{
    // put your main code here, to run repeatedly:
    //Serial.println("Hello Computer");
    //delay(500);
}