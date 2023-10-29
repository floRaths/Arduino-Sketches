// two arrays holding the noise data for colors and luminosity
uint8_t lumNoise[kMatrixHeight][kMatrixHeight];
uint8_t colNoise[kMatrixHeight][kMatrixHeight];

void makeNoise()
{
    uint32_t lumTime = millis() * hurry;
    uint32_t colTime = millis() * hurry / 2;

    memset(lumNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)lumNoise,
        kMatrixHeight,     // width
        kMatrixHeight,     // height
        1,                 // octaves
        xyVals[0],         // x
        lumRampX.update(), // scalex
        xyVals[1],         // y
        lumRampY.update(), // scaley
        lumTime            // timeVal
    );

    memset(colNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)colNoise,
        kMatrixHeight,     // width
        kMatrixHeight,     // height
        1,                 // octaves
        xyVals[2],         // x
        colRampX.update(), // scalex
        xyVals[3],         // y
        colRampY.update(), // scalex
        colTime            // timeVal
    );

    CRGBPalette16 runPal = CRGBPalette16(col[0], col[1], col[2], col[3]);

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

                uint8_t oldColData = colNoise[x][y];
                uint8_t newColData = scale8(oldColData, 256 - dataSmoothing) +
                                     scale8(colNoise[x][y], dataSmoothing);
                colNoise[x][y] = newColData;
            }

            leds[XY(x, y)] = ColorFromPalette(runPal,
                                              //  noiseCols[(y * kMatrixWidth) + x], // when used with 1D colors
                                              colNoise[x][y] + paletteIndex,
                                              // lumNoise[x][y]);
                                              //map8(lumNoise[x][y], 0, CurrentBri));
                                              brighten8_raw(map8(lumNoise[x][y], 0, CurrentBri)));
        }
    }
    

}