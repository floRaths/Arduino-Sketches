// // slowly raises brightness during startup
// void startUp()
// {
//     if (start_up == false)
//     {
//         brightnessAreaButton();

//         buildPalette();
//         for (uint8_t i = 0; i < 4; i++)
//         {
//             col[i] = pal[i];
//         }

//         briRamp.go(CurrentBri, 1000, LINEAR);
//         start_up = true;
//     }
// }

float random_float(float minValue, float maxValue)
{
    float randomNumber = minValue + (random(0, 1001) / 1000.0) * (maxValue - minValue);
    return randomNumber;
}

void changeScales(int speed)
{
    lumRampX.go(random(noiRampMin[0], noiRampMax[0]), speed * random_float(0.5, 1.5), BACK_INOUT); // when X is low and Y high = banding over height
    lumRampY.go(random(noiRampMin[1], noiRampMax[1]), speed * random_float(0.5, 1.5), BACK_INOUT); // when Y is low and X high = banding over width
    colRampX.go(random(noiRampMin[2], noiRampMax[2]), speed * random_float(0.5, 1.5), BACK_INOUT);
    colRampY.go(random(noiRampMin[3], noiRampMax[3]), speed * random_float(0.5, 1.5), BACK_INOUT);
}

void pickNewHues(uint8_t exclude_min, uint8_t exclude_max, uint8_t min_diff)
{
    uint8_t minValue = 0;
    uint8_t maxValue = 255;

    do
    {
        base_hue1 = random(minValue, maxValue + 1);     // Generate a random integer for the first variable
        do
        {
            base_hue2 = random(minValue, maxValue + 1);     // Generate a random integer for the second variable
        } while (abs(base_hue2 - base_hue1) < min_diff);    // Ensure a difference of at least 20
    } while (
        (base_hue1 >= exclude_min && base_hue1 <= exclude_max) || // Check exclusion range for the first variable
        (base_hue2 >= exclude_min && base_hue2 <= exclude_max)    // Check exclusion range for the second variable
    );
}