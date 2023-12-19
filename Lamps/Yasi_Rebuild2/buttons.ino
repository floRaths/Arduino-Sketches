uint8_t switchBrightness = 0;
uint8_t switchArea;

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

    //switchArea = (switchArea + 1) % 3;
    lowerRamp.go(lower, lo_speed, CIRCULAR_INOUT);
    upperRamp.go(upper, up_speed, CIRCULAR_INOUT);
}

// Picks new colors, then triggers color blending and goes dark,
// to signal that the palette has been changed
void paletteButton()
{
    newScales();
    pickNewHues(60, 60, 30);
    range = random(5, 15);

    palette_changed = true;
    triggerBlend(500, 500, false);

    stored_bri = CurrentBri;
    briRamp.go(0, 100, LINEAR);
}

// // Activates smooth blending to new brightness
// void brightnessButton()
// {
//     switchBrightness = (switchBrightness + 1) % 3;
//     briRamp.go(TargetBri, 1000, CIRCULAR_INOUT);
// }

// // Activates smooth blending to new area
// void areaButton()
// {
//     switchArea = (switchArea + 1) % 3;
//     lowerRamp.go(lower, 750, CIRCULAR_INOUT);
//     upperRamp.go(upper, 750, CIRCULAR_INOUT);
// }