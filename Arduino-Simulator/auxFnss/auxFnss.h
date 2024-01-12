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
    colorType col[4];
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

void generateNewHues(palette &palette, const uint8_t minDistance, bool reporting = false)
{
    uint8_t hue_old = palette.hueA;

    do // Generate the first color
    {
        palette.hueA = random(256);
    } while (testDistance(palette.hueA, hue_old, minDistance));

    do // Generate the second color
    {
        palette.hueB = random(256);
    } while (testDistance(palette.hueB, palette.hueA, minDistance));

    do // Generate the third color
    {
        palette.hueC = random(256);
    } while (testDistance(palette.hueC, palette.hueA, minDistance) || testDistance(palette.hueC, palette.hueB, minDistance));

    do // Generate the fourth color
    {
        palette.hueD = random(256);
    } while (testDistance(palette.hueD, palette.hueA, minDistance) || testDistance(palette.hueD, palette.hueB, minDistance) || testDistance(palette.hueD, palette.hueC, minDistance));

    if (reporting)
    {
        Serial.println();
        Serial.print("Hues (A,B,C,D): ");
        Serial.print(palette.hueA);
        Serial.print(", ");
        Serial.print(palette.hueB);
        Serial.print(", ");
        Serial.print(palette.hueC);
        Serial.print(", ");
        Serial.println(palette.hueD);
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
//void assemblePalette(CRGB *newColors, colorType colorInstruction[4], bool change_all, bool reporting = false) 
void assemblePalette(palette &palette, bool change_all, bool reporting = false)
{
    for (int i = 0; i < 4; ++i)
    {
        if (change_all || random(10) % 2 == 0)
        {
            if (reporting) { Serial.print("Pal" + String(i) + " = ");}
            palette.newCol[i] = colorFromRange(palette.col[i].hue,
                                            palette.col[i].hueFluct,
                                            palette.col[i].satMin,
                                            palette.col[i].satMax,
                                            palette.col[i].briMin,
                                            palette.col[i].briMax,
                                            reporting);
        }
    }
}

ramp blendRamp1, blendRamp2;
bool blending, stored_colors;

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

// Function to blend colors upon request
void blendColors(palette &palette, bool shuffle = true, bool change_all = true, bool reporting = false)
{
    // some operations that only happen during first call of this function
    if (blending == true && stored_colors == false)
    {
        assemblePalette(palette, change_all, reporting);
        // if requested, the colors will be shuffled
        if (shuffle)
        {
            shuffleColors(palette.newCol);
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
        }
}

uint8_t mtx(uint8_t x, uint8_t y)
{
    // any out of bounds address maps to the first hidden pixel
    if ((x >= MatrixX) || (y >= MatrixY))
    {
        return (NUM_LEDS);
    }

    uint8_t i;

    if (coil == true)
    {
        i = (y * MatrixX) + x;
    }
    else
    {
        if (ser_col == true)
        {
            // Even columns, counting from top to bottom
            if (x % 2 == 0)
            {
                i = x * MatrixY + y;
            }
            // Odd columns, counting from bottom to top
            else
            {
                i = x * MatrixY + (MatrixY - 1 - y);
            }
        }
        // otherwise we operate on rows (Y values)
        else
        {
            // Even rows, counting from left to right
            if (y % 2 == 0)
            {
                i = y * MatrixX + x;
            }
            // Odd rows, counting from right to left
            else
            {
                i = y * MatrixY + (MatrixY - 1 - x);
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
//void changeScales(scaleLimits lumScales, scaleLimits colScales, int speed, bool change_all, bool reporting) 
void changeScales(scales scales, int speed, bool change_all, bool reporting)
{
    if (change_all || random(10) % 2 == 0)
    {
        lumRampX.go(random(scales.lumScales.xMin, scales.lumScales.xMax), speed * random_float(0.5, 1.5), BACK_INOUT);
        lumRampY.go(random(scales.lumScales.yMin, scales.lumScales.yMax), speed * random_float(0.5, 1.5), BACK_INOUT);
        colRampX.go(random(scales.colScales.xMin, scales.colScales.xMax), speed * random_float(0.5, 1.5), BACK_INOUT);
        colRampY.go(random(scales.colScales.yMin, scales.colScales.yMax), speed * random_float(0.5, 1.5), BACK_INOUT);

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

bool executed = false;
uint8_t paletteIndex, switchPalette;

void incrementPalette(palette &palette)
{
    switchPalette = (switchPalette + 1) % (sizeof(paletteNames) / sizeof(paletteNames[0]));
    palette.paletteType = paletteNames[switchPalette];
    executed = false;
}