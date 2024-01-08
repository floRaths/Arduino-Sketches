


// CHSV makeColors(uint8_t hue_input, uint8_t hue_fluct, uint8_t sat_range[2], uint8_t bri_range[2]) {
//     uint8_t hue = random(hue_input - hue_fluct, hue_input + hue_fluct + 1);
//     uint8_t sat = random(sat_range[0], sat_range[1] + 1);
//     uint8_t bri = random(bri_range[0], bri_range[1] + 1);

//     CHSV color = CHSV(hue, sat, bri);
//     return color;
// }

uint8_t base_hue1 = 10;
uint8_t base_hue2 = 10;
uint8_t base_hue3 = 10;
uint8_t base_hue4 = 10;

CRGB newCol[4];

struct Range
{
    uint8_t min;
    uint8_t max;
};

CHSV makeColors(uint8_t baseHue, uint8_t hueFluctuation, const Range &satRange, const Range &briRange)
{
    // Validate ranges
    if (satRange.min > satRange.max || briRange.min > briRange.max)
    {
        // Handle error or provide a default value
        // For now, print an error message and return a default color
        Serial.println("Error: Invalid saturation or brightness range");
        return CHSV(baseHue, 255, 255);
    }

    uint8_t hue = random(baseHue - hueFluctuation, baseHue + hueFluctuation + 1);
    uint8_t sat = random(satRange.min, satRange.max + 1);
    uint8_t bri = random(briRange.min, briRange.max + 1);

    CHSV color = CHSV(hue, sat, bri);
    return color;
}

void buildPalette(uint8_t range, bool change_all, bool triple, uint8_t sat_range, uint8_t bri_range)
{
    for (int i = 0; i < 4; ++i)
    {
        if (change_all || random(10) % 2 == 0)
        {
            Serial.print("Pal" + String(i) + " = ");

            uint8_t base_hue;
            switch (i)
            {
            case 0: base_hue = base_hue1; break;
            case 1: base_hue = base_hue1; break;
            case 2: base_hue = base_hue2; break;
            case 3: base_hue = triple ? base_hue3 : base_hue2; break;
            }

            Range satRange = {50, 100}; // Example saturation range
            Range briRange = {20, 80};  // Example brightness range

            //newCol[i] = makeColors(base_hue, range, (i == 0 || i == 3) ? sat_range * 2.3 : sat_range, (i == 0 || i == 3) ? bri_range * 3 : bri_range);
            newCol[i] = makeColors(120, 10, satRange, briRange);
        }
    }
}
