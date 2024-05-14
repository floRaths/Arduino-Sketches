// Instructions for Single CLick Button action
void buttonClick()
{
    changeBrightness(1000, true, 255, true);
    changeStripRange(strp, false, true);
}

// Instructions for Hold Button action
void buttonHold()
{
    has_been_pressed = true;

    Serial.println();
    Serial.println("#### Palette Button ####");

    anythingGoes = (anythingGoes + 1) % 3;
    Serial.print("anythingGoes = ");
    Serial.println(anythingGoes);

    changeBrightness(125, false, 0, true);

    generateNewHues(pllt, 30, false, true);
    updatePalette  (pllt, pllt.paletteType, false, true, true);
    triggerBlend   (pllt, 750, true, true);

    changeScales(scls, 2000, true, true);

    palette_changed = true;
}