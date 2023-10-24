// void moveRange(uint8_t lower, uint8_t upper, uint8_t steps)
// {
//     for (int i = upper; i < NUM_LEDS; i++)
//     {
//         int value = (255 / steps) * (i - upper);
//         if (value >= 255)
//             value = 255;
        
//         leds[i].subtractFromRGB(value);
//     }

//     for (int k = lower; k > -1; k--)
//     {
//         int value = (255 / steps) * (lower - k);
//         if (value >= 255)
//             value = 255;
        
//         leds[k].subtractFromRGB(value);
//     }
// }

// USE THIS WHILE PROTOTYPING
void moveRange(uint8_t lower, uint8_t upper, uint8_t steps)
{
    for (int i = upper; i < NUM_LEDS; i++)
    {
        int value = (255 / steps) * (i - upper);
        if (value >= 255)
            value = 255;
    
        leds[NUM_LEDS - 1 - i].subtractFromRGB(value);
    }

    for (int k = lower; k > -1; k--)
    {
        int value = (255 / steps) * (lower - k);
        if (value >= 255)
            value = 255;
    
        leds[NUM_LEDS - 1 - k].subtractFromRGB(value);
    }
}