float random_float(float minValue, float maxValue)
{
    float randomNumber = minValue + (random(0, 1001) / 1000.0) * (maxValue - minValue);
    return randomNumber;
}

void newScales() {
    
    int randomNumber1 = random(1, 13);

    // Set the value based on probabilities
    if (randomNumber1 <= 9)
    {
        noiRampMin[0] = 5000; noiRampMax[0] = 50000;
        noiRampMin[1] = 5000; noiRampMax[1] = 50000;
        Serial.println("Regular Luma");
    }
    else if (randomNumber1 <= 11)
    {
        noiRampMin[0] = 7500; noiRampMax[0] = 30000;
        noiRampMin[1] = 1; noiRampMax[1] = 1;
        Serial.println("Side banded Luma");
    }
    else
    {
        noiRampMin[0] = 1; noiRampMax[0] = 1;
        noiRampMin[1] = 25000; noiRampMax[1] = 65000;
        Serial.println("Long banded Luma");
    }

    int randomNumber2 = random(1, 13);

    // Set the value based on probabilities
    if (randomNumber2 <= 9)
    {
        noiRampMin[2] = 5000; noiRampMax[2] = 50000;
        noiRampMin[3] = 5000; noiRampMax[3] = 50000;
        Serial.println("Regular Cols");
    }
    else if (randomNumber2 <= 11)
    {
        noiRampMin[2] = 7500; noiRampMax[2] = 30000;
        noiRampMin[3] = 1; noiRampMax[3] = 1;
        Serial.println("Side banded Cols");
    }
    else
    {
        noiRampMin[2] = 1; noiRampMax[2] = 1;
        noiRampMin[3] = 25000; noiRampMax[3] = 65000;
        Serial.println("Long banded Cols");
    }
}

void changeScales(int speed)
{
    lumRampX.go(random(noiRampMin[0], noiRampMax[0]), speed * random_float(0.5, 1.5), BACK_INOUT); // when X is low and Y high = banding over height
    lumRampY.go(random(noiRampMin[1], noiRampMax[1]), speed * random_float(0.5, 1.5), BACK_INOUT); // when Y is low and X high = banding over width
    colRampX.go(random(noiRampMin[2], noiRampMax[2]), speed * random_float(0.5, 1.5), BACK_INOUT);
    colRampY.go(random(noiRampMin[3], noiRampMax[3]), speed * random_float(0.5, 1.5), BACK_INOUT);
    Serial.println("Scales Changed");
}

void pickNewHues(uint8_t exclude_min, uint8_t exclude_max, uint8_t min_diff)
{
    do
    {
        base_hue1 = random(0, 255 + 1); // Generate a random integer for the first variable
        do
        {
            base_hue2 = random(0, 255 + 1);              // Generate a random integer for the second variable
        } while (abs(base_hue2 - base_hue1) < min_diff); // Ensure a difference of at least 20
    } while (
        (base_hue1 >= exclude_min && base_hue1 <= exclude_max) || // Check exclusion range for the first variable
        (base_hue2 >= exclude_min && base_hue2 <= exclude_max)    // Check exclusion range for the second variable
    );

    Serial.print("Hue1: ");
    Serial.print(base_hue1);
    Serial.print(", Hue2: ");
    Serial.print(base_hue2);
    Serial.print(", delta = ");
    Serial.println(abs(base_hue1 - base_hue2));
}

CHSV makeColor(uint8_t base_hue, uint8_t fluct, uint8_t sat_range, uint8_t bri_range)
{
    uint8_t hue = random(base_hue - fluct, base_hue + fluct);
    uint8_t sat = random(255 - sat_range, 255+1);
    uint8_t bri = random(255 - bri_range, 255+1);

    Serial.print(hue);
    Serial.print(", ");
    Serial.print(sat);
    Serial.print(", ");
    Serial.print(bri);
    Serial.println("");

    CHSV color = CHSV(hue, sat, bri);
    return color;
}

void buildPalette(bool randomize)
{
    if (randomize == false)
    {
        Serial.print("Pal0 = ");
        pal[0] = makeColor(base_hue1 - random(0, 9), range, 130, 45);
        Serial.print("Pal1 = ");
        pal[1] = makeColor(base_hue1, range, 55, 15);
        Serial.print("Pal2 = ");
        pal[2] = makeColor(base_hue2, range, 55, 15);
        Serial.print("Pal3 = ");
        pal[3] = makeColor(base_hue2 - random(0, 9), range, 130, 45);
        
    }
    else // randomize
    {
        coin = random(10);
        if ((coin % 2) == 0)
        {
            Serial.print("Pal0 = ");
            pal[0] = makeColor(base_hue1 - random(0, 9), range, 130, 45);
        }
        coin = random(10);
        if ((coin % 2) == 0)
        {
            Serial.print("Pal1 = ");
            pal[1] = makeColor(base_hue1, range, 55, 15);
        }
        coin = random(10);
        if ((coin % 2) == 0)
        {
            Serial.print("Pal3 = ");
            pal[2] = makeColor(base_hue2, range, 55, 15);
        }
        coin = random(10);
        if ((coin % 2) == 0)
        {
            Serial.print("Pal2 = ");
            pal[3] = makeColor(base_hue2 - random(0, 9), range, 130, 45);
        }
    }
}

uint8_t expandAndTrack(uint8_t input, int &maxValue, uint8_t buffer)
{
    // To increase contrast, update the maximum value if the input is higher
    if (input > maxValue)
    {
        maxValue = input; // Update the maximum value to the highest found in the frame
    }

    // Expand the value accordingly to the determined max value (+ 5 to reduce clipping).
    return map(input, 0, maxValue + buffer, 0, 255);
}

void moveRange(uint8_t lower, uint8_t upper, uint8_t steps)
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