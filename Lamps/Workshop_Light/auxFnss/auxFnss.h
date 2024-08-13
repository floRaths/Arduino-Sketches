ramp briRamp, blendRamp1, blendRamp2;//, indexRamp;
bool blending, stored_colors, palette_changed;
uint8_t paletteIndex, switchPalette, switchBrightness, switchStripRange;

// Struct to hold info on color ranges in order to request certain palette types
struct colorType
{
    uint8_t hue;
    uint8_t hueFluct;
    uint8_t satMin;
    uint8_t satMax;
    uint8_t briMin;
    uint8_t briMax;
};

struct palette
{
    uint8_t hueA, hueB, hueC, hueD;
    colorType recipe[4];
    
    CRGB newCol[4];
    CRGB runCol[4];
    CRGB oldCol[4];
    
    char *paletteType;
};

struct scaleLimits
{
    int xMin;
    int xMax;
    int yMin;
    int yMax;
};

struct scales
{
    scaleLimits lumScales;
    scaleLimits colScales;
    rampLong lumRampX, lumRampY, colRampX, colRampY;
    uint32_t xyVals[4];
};

struct stripRange
{
    ramp lowerRamp, upperRamp;
    uint16_t lower_limit, upper_limit, lower_store, upper_store;
    uint16_t lower_speed, upper_speed;
};


// ########## MATRIX ##########
uint8_t mtx(uint8_t x, uint8_t y, int8_t xOffset)
{
    // Calculate the new x position based on the offset
    int8_t newX = (x + xOffset) % MatrixX;

    // Ensure newX is within bounds
    if (newX < 0)
    {
        newX += MatrixX; // Wrap around to the right side
    }

    // Check if the new coordinates are out of bounds
    if ((newX < 0) || (newX >= MatrixX) || (y >= MatrixY))
    {
        return NUM_LEDS; // Out of bounds, return sentinel value
    }

    uint8_t i;

    if (coil == true)
    {
        i = (y * MatrixX) + newX;
    }
    else
    {
        if (serpentine == true) // Even columns, counting from top to bottom
        {
            if (newX % 2 == 0)
            {
                i = newX * MatrixY + y;
            }
            else // Odd columns, counting from bottom to top
            {
                i = newX * MatrixY + (MatrixY - 1 - y);
            }
        }
        else // Otherwise, we operate on rows (Y values)
        {
            if (y % 2 == 0) // Even rows, counting from left to right
            {
                i = y * MatrixX + newX;
            }
            else // Odd rows, counting from right to left
            {
                i = y * MatrixY + (MatrixY - 1 - newX);
            }
        }
    }

    // Optionally invert the index
    if (flip == true)
    {
        i = NUM_LEDS - 1 - i;
    }
    return i;
}

// ########## COLOR BLENDING ##########
// Helper function to generate a float within a defined range
float random_float(float minValue, float maxValue)
{
    float randomNumber = minValue + (random(0, 1001) / 1000.0) * (maxValue - minValue);
    return randomNumber;
}

// Function to build a CHSV color from a predefined set of input ranges for hue, sat, bri
CHSV colorFromRange(uint8_t baseHue, uint8_t hue_fluct, uint8_t sat_min, uint8_t sat_max, uint8_t bri_min, uint8_t bri_max, bool reporting = false)
{
    // Validate ranges
    if (sat_min > sat_max || bri_min > bri_max)
    {
        // Handle error or provide a default value, for now, print an error message and return a default color
        Serial.println("Error: Invalid saturation or brightness range");
        return CHSV(baseHue, 255, 255);
    }

    uint8_t hue = random(baseHue - hue_fluct, baseHue + hue_fluct + 1);
    uint8_t sat = random(sat_min, sat_max + 1);
    uint8_t bri = random(bri_min, bri_max + 1);

    if (reporting)
    {
        Serial.print("H: ");
        Serial.print(hue);
        Serial.print(", S: ");
        Serial.print(sat);
        Serial.print(", V: ");
        
        Serial.print(bri);
        Serial.println("");
    }

    CHSV color = CHSV(hue, sat, bri);
    return color;
}

// Takes an input color array and changes its order
void shuffleColors(CRGB *inputColor)
{

    uint8_t list[] = {0, 1, 2, 3};
    CRGB tmpColor[4];
    // randomize the order of list items that will determine color order
    for (int i = 0; i < 4; ++i)
    {
        int r = random(i, 4);
        int temp = list[i];
        list[i] = list[r];
        list[r] = temp;
    }

    // store the random order in a temporary color array
    for (int i = 0; i < 4; i++)
    {
        tmpColor[i] = inputColor[list[i]];
    }

    // reassign the new order to the input array
    for (int i = 0; i < 4; i++)
    {
        inputColor[i] = tmpColor[i];
    }
}

// Assembles a new 4-color palette from a set of instructions. Can be set to only assemble random subcolors
void assemblePalette(palette &palette, bool replace_all, bool reporting = false)
{
    for (int i = 0; i < 4; ++i)
    {
        if (replace_all || random(10) % 2 == 0)
        {
            if (reporting) { Serial.print("    newCol[" + String(i) + "] = ");}
            palette.newCol[i] = colorFromRange( palette.recipe[i].hue,
                                                palette.recipe[i].hueFluct,
                                                palette.recipe[i].satMin,
                                                palette.recipe[i].satMax,
                                                palette.recipe[i].briMin,
                                                palette.recipe[i].briMax,
                                                reporting);
        }
    }
}

// triggers a color blend event
void triggerBlend(palette &palette, unsigned long blend_speed, bool shuffle = true, bool reporting = false)
{
    // reset any palette blending if needed
    blendRamp1.pause();
    blendRamp1.go(0, 0);
    blendRamp2.pause();
    blendRamp2.go(0, 0);

    // trigger can only execute when no active blending is happening
    if (blendRamp2.isFinished() == 1)
    {
        if (reporting)
        {
            Serial.println();
            Serial.println(">> triggered color blend");
        }

        // if requested, the colors will be shuffled
        if (shuffle)
        {
            shuffleColors(palette.newCol);
        }

        blending = true;
        stored_colors = false;

        // start interpolation of blending function
        blendRamp1.go(255, blend_speed * random_float(0.5, 0.95), LINEAR);
        blendRamp2.go(255, blend_speed, LINEAR);
    }
}

// Function to blend colors upon request
void blendColors(palette &palette, bool reporting = false)
{
    // some operations that only happen during first call of this function
    if (blending == true && stored_colors == false)
    {
        if (reporting)
        {
            Serial.println(">> blending initiated...");
        }

        // the previous running colors are stored in an array thats used for blending
        for (int i = 0; i < 4; i++)
        {
            palette.oldCol[i] = palette.runCol[i];
        }
        stored_colors = true; // terminates the process
    }
    
    if (blendRamp2.isRunning() == 1)
    {
        palette.runCol[0] = blend(palette.oldCol[0], palette.newCol[0], blendRamp1.update());
        palette.runCol[1] = blend(palette.oldCol[1], palette.newCol[1], blendRamp2.update());
        palette.runCol[2] = blend(palette.oldCol[2], palette.newCol[2], blendRamp1.update());
        palette.runCol[3] = blend(palette.oldCol[3], palette.newCol[3], blendRamp2.update());
    }
        // after the slower ramp is done, we terminate the color blending and re-set
        if (blendRamp2.isFinished() == 1 && blending == true)
        {   // bring blending ramps back to 0
            blendRamp1.go(0, 0, NONE);
            blendRamp2.go(0, 0, NONE);
            blending = false;
            
            if (reporting)
            {
                //Serial.println();
                Serial.println(">> blending complete!");
            }
        }
        
        if (palette_changed && blendRamp2.isFinished() == 1)
        {
            palette_changed = false;
            briRamp.go(brightnessVals[switchBrightness], 1500, LINEAR);
            
            if (reporting)
            {
                Serial.println();
                Serial.println(">> palette change complete!");
            }
        }
}


// ########## SCALE FUNCTIONS ##########
// startup function to set the initial values for the noise scaling
void initializePerlin(scales &scales, int scaleStartingPoint, int xyRandom)
{

    // the scale scaleStartingPoint determines how "dramatic" the startup animation looks
    scales.lumRampX.go(scaleStartingPoint, 0, LINEAR);
    scales.lumRampY.go(scaleStartingPoint, 0, LINEAR);
    scales.colRampX.go(scaleStartingPoint, 0, LINEAR);
    scales.colRampY.go(scaleStartingPoint, 0, LINEAR);

    // random xy values for the noise field to ensure different startup each time
    for (int i = 0; i < 4; i++)
    {
        scales.xyVals[i] = random(xyRandom);
    }
}

//void changeScales(scaleLimits lumScales, scaleLimits colScales, int speed, bool change_all, bool reporting) 
void changeScales(scales &scales, unsigned long speed, bool change_all, bool reporting = false)
{
    if (reporting)
        {
            Serial.println();
            Serial.println(">> updating scale limits");
        }

    if (change_all || random(10) % 2 == 0)
    {
        int lumX = random(scales.lumScales.xMin, scales.lumScales.xMax);
        scales.lumRampX.go(lumX, speed * random_float(0.5, 1.5), BACK_INOUT);
        if (reporting)
        {
            Serial.print("lumX = ");
            Serial.print(lumX);
            Serial.print(", ");
        }
    }
    if (change_all || random(10) % 2 == 0)
    {
        int lumY = random(scales.lumScales.yMin, scales.lumScales.yMax);
        scales.lumRampY.go(lumY, speed * random_float(0.5, 1.5), BACK_INOUT);
        if (reporting)
        {
            Serial.print("lumY = ");
            Serial.print(lumY);
            Serial.print(", ");
        }
    }
    if (change_all || random(10) % 2 == 0)
    {
        int colX = random(scales.colScales.xMin, scales.colScales.xMax);
        scales.colRampX.go(colX, speed * random_float(0.5, 1.5), BACK_INOUT);
        if (reporting)
        {
            Serial.print("colX = ");
            Serial.print(colX);
            Serial.print(", ");
        }
    }
    if (change_all || random(10) % 2 == 0)
    {
        int colY = random(scales.colScales.yMin, scales.colScales.yMax);
        scales.colRampY.go(colY, speed * random_float(0.5, 1.5), BACK_INOUT);
        if (reporting)
        {
            Serial.print("colY = ");
            Serial.println(colY);
        }
    }
}


void changeBrightness(int bri_speed, bool increment = false, uint8_t targetBri = 255, bool reporting = false)
{
    if (increment)
    {
        switchBrightness = (switchBrightness + 1) % (sizeof(brightnessVals) / sizeof(brightnessVals[0]));
        targetBri = brightnessVals[switchBrightness];
        // if (switchBrightness == 0 | switchBrightness == 3 | switchBrightness == 6)
        // {
        //     targetBri = brightnessVals[switchBrightness];
        // }
        // else if (switchBrightness == 1 | switchBrightness == 4 | switchBrightness == 7)
        // {
        //     targetBri = brightnessVals[switchBrightness];
        // }
        // else if (switchBrightness == 2)
        // {
        //     targetBri = brightnessVals[2];
        // }
    }
    briRamp.go(targetBri, bri_speed, CIRCULAR_INOUT);

    if (reporting)
    {
        Serial.println();
        Serial.print(">> updating brightness to: ");
        Serial.println(targetBri);
    }
}


// ########## STRIP MARGINS ##########
void changeStripRange(stripRange &stripRange, bool increment = false, bool reporting = false, uint16_t upper_target = LAST_LED, uint16_t lower_target = 0, int speed = 1000)
{
    if (increment)
    {
        //switchStripRange = (switchStripRange + 1) % (sizeof(stripRangeVals) / sizeof(stripRangeVals[0]));
        //uint8_t n_steps = sizeof(brightnessVals) / sizeof(brightnessVals[0]);
        switchStripRange = (switchStripRange + 1) % 3;

        uint16_t overall_speed = 1000;

        //if (switchStripRange < (n_steps/3))
        if (switchStripRange == 0)
        {
            stripRange.upper_limit = LAST_LED;
            stripRange.lower_limit = 0;

            stripRange.upper_speed = overall_speed;
            stripRange.lower_speed = overall_speed;
        }
        //else if (switchStripRange < (n_steps/3)*2)
        else if (switchStripRange == 1)
        {
            stripRange.upper_limit = LAST_LED * 0.75;
            stripRange.lower_limit = LAST_LED * 0.25;

            stripRange.upper_speed = overall_speed;
            stripRange.lower_speed = overall_speed;
        }
        //else if (switchStripRange < (n_steps/3)*3)
        else if (switchStripRange == 2)
        {
            stripRange.upper_limit = LAST_LED;
            stripRange.lower_limit = LAST_LED * 0.8;

            stripRange.upper_speed = overall_speed;
            stripRange.lower_speed = overall_speed;
        }
    }
    else
    {
        stripRange.upper_limit = upper_target;
        stripRange.upper_speed = speed;

        stripRange.lower_limit = lower_target;
        stripRange.lower_speed = speed;
    }

    stripRange.upperRamp.go(stripRange.upper_limit, stripRange.upper_speed, CIRCULAR_INOUT);
    stripRange.lowerRamp.go(stripRange.lower_limit, stripRange.lower_speed, CIRCULAR_INOUT);

    if (reporting)
    {
        Serial.println();
        Serial.print(">> updating strip ranges to: ");
        Serial.print(stripRange.lower_limit);
        Serial.print(" & ");
        Serial.println(stripRange.upper_limit);
    }
}

void updateRange(uint8_t lower, uint8_t upper, uint8_t steps)
{
    for (int i = upper; i < NUM_LEDS; i++)
    {
        int value = (255 / steps) * (i - upper);
        if (value >= 255)
            value = 255;

        if (prototyping)
        {
            leds[NUM_LEDS - 1 - i].subtractFromRGB(value);
        }
        else
        {
            leds[i].subtractFromRGB(value);
        }
    }

    for (int k = lower; k > -1; k--)
    {
        int value = (255 / steps) * (lower - k);
        if (value >= 255)
            value = 255;

        if (prototyping)
        {
            leds[NUM_LEDS - 1 - k].subtractFromRGB(value);
        }
        else
        {
            leds[k].subtractFromRGB(value);
        }
    }
}


// ########## PALETTE GENERATION ##########
// Helper function to test if two hues are separate by a desired distance
bool testDistance(uint8_t first_hue, uint8_t second_hue, uint8_t minDistance)
{
    uint8_t test1 = first_hue - second_hue;
    uint8_t test2 = second_hue - first_hue;
    return (test1 <= minDistance) || (test2 <= minDistance);
}



    
    // green_low = 75;
    // green_hih = 115;

    // do
    // {
    //     hueA = random(256);
    //     hueB = random(256);
    //     hueC = random(256);
    //     hueD = random(256);
        
    // } while (hueA > green_low && hueA < green_hih &&
    //          hueB > green_low && hueB < green_hih &&
    //          hueC > green_low && hueC < green_hih &&
    //          hueD > green_low && hueD < green_hih);



// randomizes the hues of the palette based on defined constraints
void generateNewHues(palette &palette, const uint8_t minDistance, bool exclude_green = false, bool reporting = false, uint8_t green_low = 96, uint8_t green_hih = 96)
{
    uint8_t hueA, hueB, hueC, hueD;

    if (exclude_green == true)
    {
        green_low = 70;
        green_hih = 120;
    }

    uint8_t hue_old = palette.hueA;
    do
    {
        do // Generate the first color
        {
            hueA = random(256);
        } while (testDistance(hueA, hue_old, minDistance));

        do // Generate the second color
        {
            hueB = random(256);
        } while (testDistance(hueB, hueA, minDistance));

        do // Generate the third color
        {
            hueC = random(256);
        } while (testDistance(hueC, hueA, minDistance) && testDistance(hueC, hueB, minDistance));

        do // Generate the fourth color
        {
            hueD = random(256);
        } while (testDistance(hueD, hueA, minDistance) && testDistance(hueD, hueB, minDistance) && testDistance(hueD, hueC, minDistance));
    } while ((hueA >= green_low && hueA <= green_hih) ||
             (hueB >= green_low && hueB <= green_hih) ||
             (hueC >= green_low && hueC <= green_hih) ||
             (hueD >= green_low && hueD <= green_hih));

    palette.hueA = hueA;
    palette.hueB = hueB;
    palette.hueC = hueC;
    palette.hueD = hueD;

    if (reporting)
    {
        Serial.println();
        if (exclude_green)
        {
            Serial.println(">> randomized hues, avoiding green: ");
        }
        else
        {
            Serial.println(">> randomized hues: ");
        }
        Serial.print("    Hues A,B,C,D: ");
        Serial.print(palette.hueA);
        Serial.print(", ");
        Serial.print(palette.hueB);
        Serial.print(", ");
        Serial.print(palette.hueC);
        Serial.print(", ");
        Serial.println(palette.hueD);
    }
}


// assigns color instruction to the four palette colors based on requested palette type
// void updatePalette(palette &palette, const String &paletteType, bool increment = false, bool new_hues = false, bool exclude_green = false, bool reporting = false)
void updatePalette(palette &palette, const String &paletteType, bool increment = false, bool replace_all = true, bool reporting = false)
{
    if (increment)
    {
        switchPalette = (switchPalette + 1) % (sizeof(paletteNames) / sizeof(paletteNames[0]));
        paletteType = paletteNames[switchPalette];
    }

    if (paletteType == "monochrome")
    {
        palette.recipe[0] = {palette.hueA, 10, 75, 255, 155, 255};
        palette.recipe[1] = {palette.hueA, 10, 155, 255, 55, 255};
        palette.recipe[2] = {palette.hueA, 10, 155, 255, 55, 255};
        palette.recipe[3] = {palette.hueA, 10, 75, 255, 155, 255};
    }
    else if (paletteType == "duotone")
    {
        palette.recipe[0] = {palette.hueA, 10, 100, 255, 100, 255};
        palette.recipe[1] = {palette.hueA, 10, 150, 255, 220, 255};
        palette.recipe[2] = {palette.hueB, 10, 150, 255, 220, 255};
        palette.recipe[3] = {palette.hueB, 10, 100, 255, 100, 255};
    }
    else if (paletteType == "tricolore")
    {
        palette.recipe[0] = {palette.hueA, 10,  85, 255, 150, 255};
        palette.recipe[1] = {palette.hueA, 10, 155, 255, 150, 255};
        palette.recipe[2] = {palette.hueC, 10, 155, 255, 150, 255};
        palette.recipe[3] = {palette.hueB, 10, 155, 255, 150, 255};
    }
    else if (paletteType == "startUp")
    {
        palette.recipe[0] = {palette.hueA, 2, 150, 200, 255, 255};
        palette.recipe[1] = {palette.hueA, 2, 150, 200, 255, 255};
        palette.recipe[2] = {palette.hueB, 2, 150, 200, 255, 255};
        palette.recipe[3] = {palette.hueB, 2, 150, 200, 255, 255};
        // palette.recipe[0] = {palette.hueA, 10, 180, 200, 100, 255};
        // palette.recipe[1] = {palette.hueA, 10, 180, 250, 220, 255};
        // palette.recipe[2] = {palette.hueB, 10, 180, 250, 220, 255};
        // palette.recipe[3] = {palette.hueB, 10, 180, 200, 100, 255};
    }
    // else if (paletteType == "pastel")
    // {
    //     palette.recipe[0] = {palette.hueA, 0, 0, 0, 255, 255};
    //     palette.recipe[1] = {palette.hueA, 0, 0, 0, 255, 255};
    //     palette.recipe[2] = {palette.hueA, 0, 0, 0, 255, 255};
    //     palette.recipe[3] = {palette.hueA, 0, 0, 0, 255, 255};
    // }
    // else if (paletteType == "pastelAccent")
    // {
    //     palette.recipe[0] = {palette.hueA, 10, 50, 100, 200, 255};
    //     palette.recipe[1] = {palette.hueB, 10, 150, 255, 200, 255};
    //     palette.recipe[2] = {palette.hueA, 10, 50, 100, 200, 255};
    //     palette.recipe[3] = {palette.hueA, 10, 50, 100, 200, 255};
    // }
    else if (paletteType == "static")
    {
        for (int i = 0; i < 4; i++)
        {
            palette.recipe[i] = {palette.hueA, 0, 250, 250, 255, 255};
        }
    }
    else // "Unknown palette type"
    {
        palette.recipe[0] = {palette.hueA, 10, 1, 255, 1, 255};
        palette.recipe[1] = {palette.hueB, 10, 1, 255, 1, 255};
        palette.recipe[2] = {palette.hueC, 10, 1, 255, 1, 255};
        palette.recipe[3] = {palette.hueD, 10, 1, 255, 1, 255};
    }

    if (reporting)
    {
        Serial.println();
        if (increment) 
        {
            Serial.print(">> switching to ");
            Serial.print(paletteType);
            Serial.println(" palette");
        } else {
            Serial.print(">> updating ");
            Serial.print(paletteType);
            Serial.println(" palette");
        }
    }

    assemblePalette(palette, replace_all, reporting);

}



// // ########## TEST FUNCTIONS ##########
// // test function to increment hues to determine undesired values
// void findUglyHues(palette &palette, int increment)
// {
//     uint8_t index = palette.hueA;
//     index = index + increment;
//     Serial.println(index);

//     palette.hueA = index;
//     updatePalette(palette, "static", false, true, true);
//     triggerBlend(palette, 250, true, true);
// }

// // helper function to reveal the center of the matrix
void showCenter(uint8_t xOffset = 0)
{
    for (int y = 0; y < MatrixY; y++)
    {
        leds[mtx(MatrixX / 2, y, xOffset)] = CRGB::Blue;
    };
    for (int x = 0; x < MatrixX; x++)
    {
        leds[mtx(x, MatrixY / 2, xOffset)] = CRGB::Blue;
    };
    leds[mtx(MatrixX / 2, MatrixY / 2, xOffset)] = CRGB::Red;
}

// // helper fucntion to test if two colors are the same
// bool isColorEqual(const CRGB &color1, const CRGB &color2)
// {
//     return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
// }


void blackout()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    // Check if the pixel is in the middle row and is not part of blackout area
    if (i >= 0 & i <= 10)
    {
      continue; // Skip this pixel
    }
    // if (i >= 45 & i <= 55)
    // {
    //   continue; // Skip this pixel
    // }
    if (i >= 90 & i <= 100)
    {
      continue; // Skip this pixel
    }
    
    leds[i] = CRGB::Black; // Set the pixel to black
  }
}