bool isPressed, has_been_pressed;

// Instructions for Single CLick Button action
void buttonClick()
{   
    changeBrightness(1000, true, 255, true);
    changeStripRange(strp, true, true);
}

// Instructions for Hold Button action
void buttonHold()
{
    uint8_t section;
    has_been_pressed = true;
    isPressed = true;

    Serial.println();
    Serial.println("#### Palette Button ####");

    strp.upper_store = strp.upper_limit;
    strp.lower_store = strp.lower_limit;

    anythingGoes = (anythingGoes + 1) % 3;
    if (anythingGoes == 2)
    {
        section = NUM_LEDS / 2,
        changeStripRange(strp, false, true, section, section, 250);
        changeBrightness(500, false, 0, true);
    }
    else if (anythingGoes == 1)
    {
        section = 0,
        changeStripRange(strp, false, true, section, section, 250);
        changeBrightness(500, false, 0, true);
    }
    else if (anythingGoes == 0)
    {
        section = NUM_LEDS,
        changeStripRange(strp, false, true, section, section, 250);
        changeBrightness(500, false, 0, true);
    }

    generateNewHues(pllt, 30, false, true);
    updatePalette  (pllt, pllt.paletteType, false, true, true);
    triggerBlend   (pllt, 750, true, true);
    changeScales(scls, 2000, true, true);

    palette_changed = true;
}