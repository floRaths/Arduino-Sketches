
// two arrays holding the noise data for colors and luminosity


void makeNoise()
{
    memset(lumNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)lumNoise,
        kMatrixHeight,   // width
        kMatrixHeight,   // height
        2,               // octaves
        1,            // x
        5000,           // scalex
        1,            // y
        5000,           // scaley
        millis() * hurry // timeVal
    );

    CRGBPalette16 runPal = CRGBPalette16(CRGB::White, CRGB::White, CRGB::White, CRGB::White);

    for (int x = 0; x < kMatrixWidth; x++)
    {
        for (int y = 0; y < kMatrixHeight; y++)
        {
            if (dataSmoothing > 0)
            {
                uint8_t oldLumData = lumNoise[x][y];
                uint8_t newLumData = scale8(oldLumData, 256 - dataSmoothing) +
                                     scale8(lumNoise[x][y], dataSmoothing);
                lumNoise[x][y] = newLumData;
            }

            int newValue = lumNoise[x][y];
            newValue = qadd8(newValue, scale8(newValue, 200));
            // Update the current minimum and maximum values
            if (newValue < currentMin)
            {
                currentMin = newValue;
            }
            if (newValue > currentMax)
            {
                currentMax = newValue;
            }

            leds[XY(x, y)] = ColorFromPalette(runPal, 128,
                                              // lumNoise[x][y]);
                                              //map8(lumNoise[x][y], 0, CurrentBri));
                                              brighten8_raw(lumNoise[x][y]));
        }
    }



    // for (int row = 0; row < kMatrixHeight; row++)
    // {
    //     // Serial.println(leds[XY(4, 10)].getLuma());
    //     Serial.print(lumNoise[4][row]);
    //     Serial.print(", ");
    //     // Serial.println(lumNoise[4][10]);
    // } Serial.println();
    // // Serial.println(brighten8_raw(map8(lumNoise[4][10], 0, 255)));
}