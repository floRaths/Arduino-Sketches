
uint8_t switchBrightness;
uint8_t switchArea;
uint8_t switchPalette;
uint8_t switchPalette2;

uint8_t stored_bri;
uint8_t TargetBri; // when the button is pressed, this is the new brightness to transition to

void buttonSwitches()
{
    CurrentBri = briRamp.update();
    switch (switchBrightness)
    {
    case 0:
        TargetBri = Bri1;
        break;
    case 1:
        TargetBri = Bri2;
        break;
    case 2:
        TargetBri = Bri3;
        break;
    }
}

void brightnessAreaButton(uint8_t brightness, int bri_speed, int lo_speed, int up_speed)
{
    switchBrightness = (switchBrightness + 1) % 3;
    briRamp.go(brightness, bri_speed, CIRCULAR_INOUT);

    switchArea = (switchArea + 1) % 3;
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
    triggerBlend(500, 500, true);
    
    stored_bri = CurrentBri;
    briRamp.go(0, 50, LINEAR);
    
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