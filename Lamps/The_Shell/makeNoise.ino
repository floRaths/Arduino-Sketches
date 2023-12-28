// two arrays holding the noise data for colors and luminosity
uint8_t lumNoise[kMatrixHeight][kMatrixHeight];
uint8_t scaledData[kMatrixHeight][kMatrixHeight];
uint8_t data[kMatrixHeight][kMatrixHeight];

uint8_t colNoise[kMatrixHeight][kMatrixHeight];
uint8_t CscaledData[kMatrixHeight][kMatrixHeight];
uint8_t Cdata[kMatrixHeight][kMatrixHeight];

int maxValue = 0; // Assume the first element is the maximum

void makeNoise()
{
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
        millis() * hurry   // timeVal
    );

    memset(colNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)colNoise,
        kMatrixHeight,       // width
        kMatrixHeight,       // height
        1,                   // octaves
        xyVals[2],           // x
        colRampX.update(),   // scalex
        xyVals[3],           // y
        colRampY.update(),   // scalex
        millis() * 4 // timeVal
    );

    CRGBPalette16 runPal = CRGBPalette16(runCol[0], runCol[1], runCol[2], runCol[3]);

    for (int x = 0; x < kMatrixWidth; x++)
    {
        for (int y = 0; y < kMatrixHeight; y++)
        {
            if (lumNoise[x][y] > maxValue)
            {
                maxValue = lumNoise[x][y]; // Update the maximum value to the highest found in the frame
            }

            scaledData[x][y] = qadd8(lumNoise[x][y], scale8(lumNoise[x][y], maxValue));

            if (dataSmoothing)
            {
                uint8_t  olddata = data[x][y];
                uint16_t newdata = (uint16_t)olddata * (256 - 128) + (uint16_t)scaledData[x][y] * 128;
                data[x][y] = newdata / 256; // Convert back to uint8_t

                uint8_t Colddata = Cdata[x][y];
                uint16_t Cnewdata = (uint16_t)Colddata * (256 - 128) + (uint16_t)colNoise[x][y] * 128;
                Cdata[x][y] = Cnewdata / 256; // Convert back to uint8_t
            }
            else
            {
                data[x][y] = scaledData[x][y];
                Cdata[x][y] = colNoise[x][y];
            }

            leds[mtx(x, y)] = ColorFromPalette(runPal,
                                               // noiseCols[(y * kMatrixWidth) + x], // when used with 1D colors
                                               // colNoise[x][y] + paletteIndex,
                                               Cdata[x][y] + paletteIndex,
                                               // data[x][y]);
                                               brighten8_lin(data[x][y]));
            // dim8_raw(data[x][y]));
        }
    }
}