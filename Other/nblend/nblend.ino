#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define NUM_LEDS 120
#define BTN_PIN 7
CRGB leds[NUM_LEDS];

OneButton btn = OneButton(BTN_PIN, true, true);

boolean rolling = false;
boolean said_it = false;
uint8_t blendo = 0;

CRGB runCol;
CRGB oldCol;
CRGB newCol;
CRGB tmpCol;

//float blendo = 0.0;

rampInt palRamp2;

CRGB color1 = CRGB(255, 0, 0); // Red
CRGB color2 = CRGB(0, 0, 255); // Blue

int blendFactor = 0;

void setup()
{
    FastLED.addLeds<WS2812, 5, GRB>(leds, NUM_LEDS);
    Serial.begin(9600);
    oldCol = CRGB::Red;
    newCol = CRGB::Blue;
    runCol = oldCol;

    btn.attachClick(button);
}

void loop()
{
    // if (rolling)
    // {
    //     Serial.println("rolling ...");
    //     rolling = false;
    //     said_it = false;
    // }

    // uint8_t sinBeat = beatsin8(30, 0, 255, 0, 0);

    // //col[0] = nblend(col[0], col[1], palRamp2.update());
    // col[0] = blend(col[3], col[1], sinBeat);

    EVERY_N_SECONDS(5)
    {
        rolling = true;
        palRamp2.go(255, 1000, LINEAR);
    }

    // if (isColorEqual(col[0], col[1]) && said_it == false)
    // {
    //     Serial.println("equal");
    //     //Serial.println(palRamp2.update());
    //     //Serial.println(blendo);
    //     //blendo = 0;
    //     said_it = true;
    // }

    // EVERY_N_MILLISECONDS(10)
    // {
    //     //blendo++;
    //     Serial.println(sinBeat);
    // }

    //if (palRamp2.isRunning() == 1 && rolling == true) {
    runCol = blend(oldCol, newCol, palRamp2.update());
    //}

    if (palRamp2.isFinished() == 1 && rolling == true)
    {
        tmpCol = newCol;
        newCol = oldCol;
        oldCol = tmpCol;

        palRamp2.go(0, 0, NONE);
        rolling = false;
    }



    EVERY_N_MILLISECONDS(250) { 
        //blendFactor = blendFactor + 1;
        Serial.print("RampOn: ");
        Serial.print(palRamp2.isRunning());
        Serial.print(" Factor: ");
        Serial.print(palRamp2.update());
        Serial.print(" Blended Color: (");
        Serial.print(runCol.r);
        Serial.print(", ");
        Serial.print(runCol.g);
        Serial.print(", ");
        Serial.print(runCol.b);
        Serial.println(")");
    }
    
    leds[0] = runCol;
    FastLED.show();
    btn.tick();
}

void button() 
{
    blendo++;
    // blendFactor <= 1.0;
    //blendo = blendo + 0.1;
    Serial.println(blendo);
}

bool isColorEqual(const CRGB &color1, const CRGB &color2)
{
    return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}
