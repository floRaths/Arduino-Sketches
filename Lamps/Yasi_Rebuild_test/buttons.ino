
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

    switch (switchPalette)
    {
    case 0: // side banded
        noiRampMin[0] = random(5000, 10000);
        noiRampMax[0] = random(10000, 50000);
        noiRampMin[1] = 1;
        noiRampMax[1] = 1;
        break;
    case 1: // vertical banded
        noiRampMin[0] = 1;
        noiRampMax[0] = 1;
        noiRampMin[1] = random(20000, 20000);
        noiRampMax[1] = random(50000, 50000);
        break;
    case 2:
        noiRampMin[0] = random(5000, 10000);
        noiRampMax[0] = random(10000, 50000);
        noiRampMin[1] = random(5000, 10000);
        noiRampMax[1] = random(10000, 50000);
        break;
    case 3:
        noiRampMin[0] = random(5000, 10000);
        noiRampMax[0] = random(10000, 25000);
        noiRampMin[1] = random(5000, 10000);
        noiRampMax[1] = random(10000, 25000);
        break;
    }
    
    switch (switchPalette2)
    {
    case 0: // side banded
        noiRampMin[2] = random(5000, 10000);
        noiRampMax[2] = random(10000, 50000);
        noiRampMin[3] = 1;
        noiRampMax[3] = 1;
        break;
    case 1: // vertical banded
        noiRampMin[2] = 1;
        noiRampMax[2] = 1;
        noiRampMin[3] = random(20000, 20000);
        noiRampMax[3] = random(50000, 50000);
        break;
    case 2:
        noiRampMin[2] = random(5000, 10000);
        noiRampMax[2] = random(10000, 50000);
        noiRampMin[3] = random(5000, 10000);
        noiRampMax[3] = random(10000, 50000);
        break;
    case 3:
        noiRampMin[2] = random(5000, 10000);
        noiRampMax[2] = random(10000, 25000);
        noiRampMin[3] = random(5000, 10000);
        noiRampMax[3] = random(10000, 25000);
        break;
    }
}

// Picks new colors, then triggers color blending and goes dark,
// to signal that the palette has been changed
void paletteButton()
{
    switchPalette  = 0;//random(4);
    switchPalette2 = 0;//random(4);
    //pickNewHues(60, 60, 30);
    //range = random(5, 15);

    palette_changed = true;
    grant_blend = true;
    speed1 = 500;
    speed2 = 500;
    stored_bri = CurrentBri;
    briRamp.go(0, 50, LINEAR);
    
}

// Activates smooth blending to new brightness
void brightnessButton()
{
    switchBrightness = (switchBrightness + 1) % 3;
    briRamp.go(TargetBri, 1000, CIRCULAR_INOUT);
}

// Activates smooth blending to new area
void areaButton()
{
    switchArea = (switchArea + 1) % 3;
    lowerRamp.go(lower, 750, CIRCULAR_INOUT);
    upperRamp.go(upper, 750, CIRCULAR_INOUT);
}

void brightnessAreaButton(uint8_t brightness, int bri_speed, int lo_speed, int up_speed)
{
    switchBrightness = (switchBrightness + 1) % 3;
    briRamp.go(brightness, bri_speed, CIRCULAR_INOUT);

    switchArea = (switchArea + 1) % 3;
    lowerRamp.go(lower, lo_speed, CIRCULAR_INOUT);
    upperRamp.go(upper, up_speed, CIRCULAR_INOUT);
}