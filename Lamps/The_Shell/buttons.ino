uint8_t switchBrightness = 0;
uint8_t switchArea = 2;
uint8_t switchIndex = 4;

uint8_t stored_bri; // when the button is pressed, this is the new brightness to transition to

void buttonSwitches()
{
    CurrentBri = briRamp.update();
    bri_speed = 750;
    lo_speed = 1000;
    up_speed = 1000;

    switch (switchBrightness)
    {
    case 0:
        if (pressed)
        {
            TargetBri = Bri1;

            lower = 0;            // lower end of lights
            upper = NUM_LEDS - 4; // upper end of lights

            pressed = false;
        }
        break;

    case 1:
        if (pressed)
        {
            TargetBri = Bri2;

            lower = 0;            // lower end of lights
            upper = NUM_LEDS - 4; // upper end of lights

            pressed = false;
        }
        break;

    case 2:

        if (pressed)
        {
            TargetBri = Bri3;

            lower = 0;  // lower end of lights
            upper = 20; // upper end of lights
        
            pressed = false;
        }
        break;
    }
}

void brightnessAreaButton()
{
    pressed = true;
    switchBrightness = (switchBrightness + 1) % 3;

    briRamp.go(TargetBri, bri_speed, CIRCULAR_INOUT);

    lowerRamp.go(lower, lo_speed, CIRCULAR_INOUT);
    upperRamp.go(upper, up_speed, CIRCULAR_INOUT);
    //changeScales(30000);
    Serial.println(noiRampMin[0]);
}

// Picks new colors, then triggers color blending and goes dark,
// to signal that the palette has been changed
void paletteButton()
{
    Serial.println();
    Serial.println("#### Palette Button ####");

    // reset any palette blending if needed
    palRamp1.pause(); palRamp1.go(0, 0);
    palRamp2.pause(); palRamp2.go(0, 0);
    
    switchArea = (switchArea + 1) % 3;
    if (switchArea == 2)
    {
        triple = true;
        Serial.println("building tricolor palette");
        
    }
    else
    {
        triple = false;
        Serial.println("building duocolor palette");
    }

    switchIndex = (switchIndex + 1) % 5;
    if (switchIndex == 4)
    {
        indexDrift = true;
        Serial.println("activating Index drift");
        }
    else
    {
        indexDrift = false;
        Serial.println("no Index drift");
    }

    if (switchIndex == 3)
    {
        rainbow = true;
        Serial.println("RAINBOWWW");
    }
    else
    {
        rainbow = false;
    }

    newHues(30);
    range = random(5, 15);
    buildPalette(range, true, triple, sat_range, bri_range);
    triggerRoll(1000);

    newScales();
    changeScales(10000);

    palette_changed = true;
    stored_bri = CurrentBri;
    briRamp.go(0, 100, LINEAR);
}