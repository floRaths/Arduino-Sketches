// two arrays holding the noise data for colors and luminosity
uint8_t lumNoise[kMatrixHeight][kMatrixHeight];
uint8_t colNoise[kMatrixHeight][kMatrixHeight];

uint32_t lumTime;
uint32_t colTime;


void makeNoise()
{
    lumTime = millis() * hurry;
    colTime = millis() * hurry;

    memset(lumNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)lumNoise,
        kMatrixHeight,            // width
        kMatrixHeight,            // height
        1,                        // octaves
        xyVals[0],                // x
        lumRampX.update(),        // scalex
        xyVals[1],                // y
        lumRampY.update(),        // scaley
        lumTime // timeVal
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
        colTime    // timeVal
    );

    CRGBPalette16 runPal = CRGBPalette16(col[1], col[3], col[2], col[0]);

    for (int x = 0; x < kMatrixWidth; x++)
    {
        for (int y = 0; y < kMatrixHeight; y++)
        {
            leds[XY(x, y)] =
                ColorFromPalette(runPal,
                                 //  noiseCols[(y * kMatrixWidth) + x],
                                 //  brighten8_raw(map8(noiseData[x][y], 0, CurrentBri)));
                                 colNoise[x][y] + paletteIndex,
                                 brighten8_raw(map8(lumNoise[x][y], 0, CurrentBri)));
            // brighten8_raw(map8(noiseCols[(y * kMatrixWidth) + x], 0, CurrentBri)));
        }
    }
}


// uint8_t noiseCols[NUM_LEDS];

// memset(noiseCols, 0, NUM_LEDS);
// fill_raw_noise16into8(
//     noiseCols,
//     NUM_LEDS,
//     1,            // octaves
//     33000,        //  x - don't mess with this one
//     NUM_LEDS * 6, // scale
//     timeCols);