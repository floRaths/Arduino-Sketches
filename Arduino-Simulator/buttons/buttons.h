// Instructions for Single CLick Button action
void buttonClick()
{
    changeBrightness(1000, true, 255, true);
    changeStripRange(strp, true, true);
}

// Instructions for Hold Button action
void buttonHold()
{
    Serial.println();
    Serial.println("#### Palette Button ####");

    // reset any palette blending if needed
    blendRamp1.pause();
    blendRamp1.go(0, 0);
    blendRamp2.pause();
    blendRamp2.go(0, 0);

    changeBrightness(125, false, 0, true);

    generateNewHues(pllt, 30, true);
    updatePalette(pllt, pllt.paletteType, true, true, true, true);
    triggerBlend(pllt, 750, true, true);

    changeScales(scls, 2000, true, true);

    palette_changed = true;
}