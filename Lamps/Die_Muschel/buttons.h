// Instructions for Single CLick Button action
void userBrightness()
{
    changeBrightness(1000, true, 255, true);
    changeStripRange(strp, true, true);
}

void userIntensity()
{
    Serial.println();
    Serial.println("#### DoubleCLick ####");
    
    
    switchIntensity = (switchIntensity + 1) % 3;
    intensity = Intensities[switchIntensity];

    updatePalette  (pllt, pllt.paletteType, false, intensity, true, true);
    triggerBlend   (pllt, 750, false, true);
    
}

// Instructions for Hold Button action
void userPalette()
{
    has_been_pressed = true;

    Serial.println();
    Serial.println("#### Palette Button ####");

    anythingGoes = (anythingGoes + 1) % 3;
    Serial.print("anythingGoes = ");
    Serial.println(anythingGoes);

    changeBrightness(125, false, 0, true);

    generateNewHues(pllt, 30, false, true);
    updatePalette  (pllt, pllt.paletteType, true, intensity, true, true);
    triggerBlend   (pllt, 750, true, true);

    //changeScales(scls, 2000, true, true);

    palette_changed = true;
}
