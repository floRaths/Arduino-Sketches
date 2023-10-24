// two arrays holding the noise data for colors and luminosity
uint8_t lumNoise[kMatrixHeight][kMatrixHeight];
uint8_t colNoise[kMatrixHeight][kMatrixHeight];

uint32_t lumTime;
uint32_t colTime;

uint8_t lumDataSmoothing = 24; // You can adjust this value

void makeNoise()
{
    lumTime = millis() * 5;
    colTime = millis() * hurry;

    // Initialize temporary noise array
    uint8_t tempLumNoise[kMatrixHeight][kMatrixHeight];

    memset(lumNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)lumNoise,
        kMatrixHeight, // width
        kMatrixHeight, // height
        1,             // octaves
        1,             // x
        10000,           // scalex
        1,             // y
        10000,           // scaley
        lumTime        // timeVal
    );

    memset(colNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)colNoise,
        kMatrixHeight, // width
        kMatrixHeight, // height
        1,             // octaves
        1,             // x
        5000,          // scalex
        1,             // y
        5000,          // scaley
        colTime        // timeVal
    );

    CRGBPalette16 runPal = CRGBPalette16(col[1], col[3], col[2], col[0]);

    for (int x = 0; x < kMatrixWidth; x++)
    {
        for (int y = 0; y < kMatrixHeight; y++)
        {

            uint8_t newData = scale8(lumNoise[x][y], 256 - lumDataSmoothing) +
                              scale8(tempLumNoise[x][y], lumDataSmoothing);
            // Update lumNoise with the new value
            lumNoise[x][y] = newData;

            leds[XY(x, y)] =
                ColorFromPalette(runPal,
                                 //  noiseCols[(y * kMatrixWidth) + x],
                                 //  brighten8_raw(map8(noiseData[x][y], 0, CurrentBri)));
                                 colNoise[x][y] + paletteIndex,
                                 lumNoise[x][y]);
            // brighten8_raw(map8(noiseCols[(y * kMatrixWidth) + x], 0, CurrentBri)));
        }
    }

    Serial.print(leds[XY(4, 10)].getLuma());
    Serial.print(lumNoise[4][10]);
    Serial.print("_");
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