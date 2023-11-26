uint8_t switchBrightness = 0;
uint8_t switchArea = 2;
uint8_t switchIndex = 4;

uint8_t stored_bri; // when the button is pressed, this is the new brightness to transition to

void buttonSwitches()
{
    CurrentBri = briRamp.update();
    switch (switchBrightness)
    {
    case 0:
        TargetBri = Bri1;
        bri_speed = 750;
        lo_speed = 1000;
        up_speed = 1000;

        lower = 0;                     // lower end of lights
        upper = NUM_LEDS - 4; // upper end of lights
        break;
    case 1:
        TargetBri = Bri2;
        bri_speed = 750;
        lo_speed = 1000;
        up_speed = 1000;

        lower = 0;            // lower end of lights
        upper = NUM_LEDS - 4; // upper end of lights
        break;
    case 2:
        TargetBri = Bri3;
        bri_speed = 750;
        lo_speed = 1000;
        up_speed = 1000;

        lower = 0;            // lower end of lights
        upper = 30; // upper end of lights
        break;
    }
}

void brightnessAreaButton()
{
    switchBrightness = (switchBrightness + 1) % 3;
    briRamp.go(TargetBri, bri_speed, CIRCULAR_INOUT);

    lowerRamp.go(lower, lo_speed, CIRCULAR_INOUT);
    upperRamp.go(upper, up_speed, CIRCULAR_INOUT);
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

    newHues(30);
    buildPalette(random(5, 15), true, triple);
    triggerRoll(1000);

    newScales();
    changeScales(6000);

    palette_changed = true;
    stored_bri = CurrentBri;
    briRamp.go(0, 100, LINEAR);
}