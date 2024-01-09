
// Helper function to test if two hues are separate by a desired distance
bool testDistance(uint8_t first_hue, uint8_t second_hue, uint8_t minDistance)
{
    uint8_t test1 = first_hue - second_hue;
    uint8_t test2 = second_hue - first_hue;
    return (test1 <= minDistance) || (test2 <= minDistance);
}

// Function to generate three random values with a defined minimum separation
void generateNewHues(uint8_t &hue_A, uint8_t &hue_B, uint8_t &hue_C, uint8_t &hue_D, const uint8_t minDistance)
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
}

uint8_t hue_A = 10;
uint8_t hue_B = 10;
uint8_t hue_C = 10;
uint8_t hue_D = 10;

uint8_t colorScheme;

CRGB newCol[4];


CHSV colorFromRange(uint8_t baseHue, uint8_t hue_fluct, uint8_t sat_min, uint8_t sat_max, uint8_t bri_min, uint8_t bri_max)
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

    CHSV color = CHSV(hue, sat, bri);
    return color;
}


struct colorProperty
{
    uint8_t hue;
    uint8_t satMin;
    uint8_t satMax;
    uint8_t briMin;
    uint8_t briMax;
};

colorProperty color[4];

void buildPalette(uint8_t hueFluct, bool change_all)
{
    for (int i = 0; i < 4; ++i)
    {
        if (change_all || random(10) % 2 == 0)
        {
            Serial.print("Pal" + String(i) + " = ");
            newCol[i] = colorFromRange(color[i].hue,
                                       hueFluct,
                                       color[i].satMin,
                                       color[i].satMax,
                                       color[i].briMin,
                                       color[i].briMax);
        }
    }
}

