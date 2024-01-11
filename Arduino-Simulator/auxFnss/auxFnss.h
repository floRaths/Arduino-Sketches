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

struct scaleLimits
{
    int xMin;
    int xMax;
    int yMin;
    int yMax;
};

float random_float(float minValue, float maxValue)
{
    float randomNumber = minValue + (random(0, 1001) / 1000.0) * (maxValue - minValue);
    return randomNumber;
}

// Helper function to test if two hues are separate by a desired distance
bool testDistance(uint8_t first_hue, uint8_t second_hue, uint8_t minDistance)
{
    uint8_t test1 = first_hue - second_hue;
    uint8_t test2 = second_hue - first_hue;
    return (test1 <= minDistance) || (test2 <= minDistance);
}

uint8_t hueA, hueB, hueC, hueD;
// Function to generate three random values with a defined minimum separation
void generateNewHues(uint8_t &hue_A, uint8_t &hue_B, uint8_t &hue_C, uint8_t &hue_D, const uint8_t minDistance, bool reporting = false)
{
    uint8_t hue_old = hue_A;

    do // Generate the first color
    {
        hue_A = random(256);
    } while (testDistance(hue_A, hue_old, minDistance));

    do // Generate the second color
    {
        hue_B = random(256);
    } while (testDistance(hue_B, hue_A, minDistance));

    do // Generate the third color
    {
        hue_C = random(256);
    } while (testDistance(hue_C, hue_A, minDistance) || testDistance(hue_C, hue_B, minDistance));

    do // Generate the fourth color
    {
        hue_D = random(256);
    } while (testDistance(hue_D, hue_A, minDistance) || testDistance(hue_D, hue_B, minDistance) || testDistance(hue_D, hue_C, minDistance));

    if (reporting)
    {
        Serial.println();
        Serial.print("Hues (A,B,C,D): ");
        Serial.print(hue_A);
        Serial.print(", ");
        Serial.print(hue_B);
        Serial.print(", ");
        Serial.print(hue_C);
        Serial.print(", ");
        Serial.println(hue_D);
    }
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
        Serial.print(hue);
        Serial.print(", ");
        Serial.print(sat);
        Serial.print(", ");
        Serial.print(bri);
        Serial.println("");
    }

    CHSV color = CHSV(hue, sat, bri);
    return color;
}

// Assembles a new 4-color palette from a set of instructions. Can be set to only assemble random subcolors
void assemblePalette(CRGB *newColors, colorType colorInstruction[4], bool change_all, bool reporting = false)
{
    for (int i = 0; i < 4; ++i)
    {
        if (change_all || random(10) % 2 == 0)
        {
            if (reporting) { Serial.print("Pal" + String(i) + " = ");}
            newColors[i] = colorFromRange(colorInstruction[i].hue,
                                          colorInstruction[i].hueFluct,
                                          colorInstruction[i].satMin,
                                          colorInstruction[i].satMax,
                                          colorInstruction[i].briMin,
                                          colorInstruction[i].briMax,
                                          reporting);
        }
    }
}

ramp blendRamp1, blendRamp2;
bool blending, stored_colors;
uint8_t list[] = {0, 1, 2, 3};
CRGB newCol[4];
CRGB runCol[4];
CRGB oldColor[4];

// triggers a color blend event
void triggerBlend(int blend_speed, bool reporting = false)
{
    if (blendRamp2.isFinished() == 1)
    {
        if (reporting)
        {
            Serial.println("triggered");
        }

        blending = true;
        stored_colors = false;

        // start interpolation of blending function
        blendRamp1.go(255, blend_speed * random_float(0.5, 0.95), LINEAR);
        blendRamp2.go(255, blend_speed, LINEAR);
    }
}

// Takes an input color array and changes its order
void shuffleColors(CRGB *inputColor) {

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

void blendColors(CRGB *runningColor, CRGB *newColor, colorType colorInstruction[4], bool shuffle = true, bool change_all = true, bool reporting = false)
//void blendColors(palette *colorPalette, colorType colorInstruction[4], bool shuffle = true, bool change_all = true, bool reporting = false)
{
    // some operations that only happen during first call of this function
    if (blending == true && stored_colors == false)
    {
        assemblePalette(newColor, colorInstruction, change_all, reporting);
        //assemblePalette(colorPalette.newCol, colorInstruction, change_all, reporting);
        // if requested, the colors will be shuffled
        if (shuffle)
        {
            shuffleColors(newColor);
            //shuffleColors(colorPalette.newCol);
        }
        // the previous running colors are stored in an array thats used for blending
        for (int i = 0; i < 4; i++)
        {
            oldColor[i] = runningColor[i];
            //colorPalette.oldCol[i] = colorPalette.runCol[i];
        }
        stored_colors = true; // terminates the process
    }
    
    if (blendRamp2.isRunning() == 1 )
    {
        runningColor[0] = blend(oldColor[0], newColor[0], blendRamp1.update());
        runningColor[1] = blend(oldColor[1], newColor[1], blendRamp2.update());
        runningColor[2] = blend(oldColor[2], newColor[2], blendRamp1.update());
        runningColor[3] = blend(oldColor[3], newColor[3], blendRamp2.update());

        // colorPalette.runCol[0] = blend(colorPalette.oldCol[0], colorPalette.newCol[0], blendRamp1.update());
        // colorPalette.runCol[1] = blend(colorPalette.oldCol[1], colorPalette.newCol[1], blendRamp2.update());
        // colorPalette.runCol[2] = blend(colorPalette.oldCol[2], colorPalette.newCol[2], blendRamp1.update());
        // colorPalette.runCol[3] = blend(colorPalette.oldCol[3], colorPalette.newCol[3], blendRamp2.update());
    }
        // after the slower ramp is done, we terminate the color blending and re-set
        if (blendRamp2.isFinished() == 1 && blending == true)
        {   // bring blending ramps back to 0
            blendRamp1.go(0, 0, NONE);
            blendRamp2.go(0, 0, NONE);
            blending = false;
        }
}

uint8_t mtx(uint8_t x, uint8_t y)
{
    // any out of bounds address maps to the first hidden pixel
    if ((x >= kMatrixWidth) || (y >= kMatrixHeight))
    {
        return (NUM_LEDS);
    }

    uint8_t i;

    if (coil == true)
    {
        i = (y * kMatrixWidth) + x;
    }
    else
    {
        if (ser_col == true)
        {
            // Even columns, counting from top to bottom
            if (x % 2 == 0)
            {
                i = x * kMatrixHeight + y;
            }
            // Odd columns, counting from bottom to top
            else
            {
                i = x * kMatrixHeight + (kMatrixHeight - 1 - y);
            }
        }
        // otherwise we operate on rows (Y values)
        else
        {
            // Even rows, counting from left to right
            if (y % 2 == 0)
            {
                i = y * kMatrixWidth + x;
            }
            // Odd rows, counting from right to left
            else
            {
                i = y * kMatrixWidth + (kMatrixWidth - 1 - x);
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

rampLong lumRampX, lumRampY, colRampX, colRampY;
void changeScales(scaleLimits lumScales, scaleLimits colScales, int speed, bool change_all, bool reporting)
{
    if (change_all || random(10) % 2 == 0)
    {
        lumRampX.go(random(lumScales.xMin, lumScales.xMax), speed * random_float(0.5, 1.5), BACK_INOUT);
        lumRampY.go(random(lumScales.yMin, lumScales.yMax), speed * random_float(0.5, 1.5), BACK_INOUT);
        colRampX.go(random(colScales.xMin, colScales.xMax), speed * random_float(0.5, 1.5), BACK_INOUT);
        colRampY.go(random(colScales.yMin, colScales.yMax), speed * random_float(0.5, 1.5), BACK_INOUT);

        if (reporting)
        {
            Serial.println("Scales Changed");
        }
    }
}

uint32_t xyVals[4];
void initializePerlin (int scaleStartingPoint, int xyRandom) {

    lumRampX.go(scaleStartingPoint, 0, LINEAR);
    lumRampY.go(scaleStartingPoint, 0, LINEAR);
    colRampX.go(scaleStartingPoint, 0, LINEAR);
    colRampY.go(scaleStartingPoint, 0, LINEAR);

    // random xy values for the noise field to ensure different starting points
    for (int i = 0; i < 4; i++)
    {
        xyVals[i] = random(xyRandom);
    }
}