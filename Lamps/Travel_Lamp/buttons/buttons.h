bool isPressed, has_been_pressed, anythingGoes, blinking;
uint8_t paletteSelector = 2;
uint8_t n_blinks, blinkCounter;

// Instructions for Single CLick Button action
void userBrightness()
{   
    changeBrightness(1000, true, 255, true);
    changeStripRange(strp, true, true);
}

// Instructions for Hold Button action
void userPalette()
{
    uint8_t section;
    has_been_pressed = true;
    isPressed = true;

    Serial.println();
    Serial.println("#### Palette Button ####");

    strp.upper_store = strp.upper_limit;
    strp.lower_store = strp.lower_limit;

    paletteSelector = (paletteSelector + 1) % 3;
    uint16_t brit_speed = 100;
    uint16_t strp_speed = 150;
    
    if (paletteSelector == 0)
    {
        pllt.paletteType = "monochrome";
        section = 0,
        changeStripRange(strp, false, true, section, section, strp_speed);
        changeBrightness(brit_speed, false, 0, true);
    }
    else if (paletteSelector == 1)
    {
        pllt.paletteType = "duotone";
        section = NUM_LEDS,
        changeStripRange(strp, false, true, section, section, strp_speed);
        changeBrightness(brit_speed, false, 0, true);
    }
    else if (paletteSelector == 2)
    {
        pllt.paletteType = "tricolore";
        section = NUM_LEDS / 2,
        changeStripRange(strp, false, true, section, section, strp_speed);
        changeBrightness(brit_speed, false, 0, true);
    }

    generateNewHues(pllt, 30, true, true);
    updatePalette  (pllt, pllt.paletteType, false, true, true);
    triggerBlend   (pllt, 750, true, true);
    changeScales(scls, 2000, true, true);

    palette_changed = true;
}


void userBlink(void *oneButton)
{
    blinking = true;
    blinkCounter = 0;
    n_blinks = 2;

    if (anythingGoes == false)
    {
        anythingGoes = true;
    }
    else
    {
        anythingGoes = false;
    }
}

void blinkAction()
{
    if (blinking == true && briRamp.getOrigin() != 0 && briRamp.isFinished() == true)
    {
        briRamp.go(0, 50, CIRCULAR_IN);
    }
    else if (blinking == true && briRamp.getOrigin() != 255 && briRamp.isFinished() == true)
    {
        briRamp.go(255, 300, CIRCULAR_OUT);
        blinkCounter = blinkCounter + 1;
    }

    if (blinkCounter == n_blinks*2)
    {
        blinking = false;
    }
}

void DuringLongPress(void *oneButton)
{
  // Serial.print(((OneButton *)oneButton)->getPressedMs());
  // Serial.println("\t - DuringLongPress()");
  EVERY_N_SECONDS(2)
  {
    Serial.println();
    Serial.println("######## LongPress Action ########");
    // generateNewHues(pllt, 15, true, true);
    // updatePalette(pllt, pllt.paletteType, false);
    // triggerBlend(pllt, 500, true, true);
    changeScales(scls, 2000, true, true);
  }
}