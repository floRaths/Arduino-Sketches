// two arrays holding the noise data for colors and luminosity
uint8_t noiseCols[NUM_LEDS];
uint8_t noiseData[kMatrixHeight][kMatrixHeight];

uint32_t timeLuma;
uint32_t timeCols;

int scalex;
int scaley;

void makeNoise()
{
    timeLuma = millis() * hurry;
    timeCols = millis() * hurry / 2;

    scalex = noiRamp1.update();
    scaley = noiRamp2.update();

    memset(noiseData, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)noiseData,
        kMatrixHeight,     // width
        kMatrixHeight,     // height
        1,                 // octaves
        5000,              // x
        noiRamp1.update(), // scalex
        1000,              // y
        noiRamp2.update(), // scaley
        timeLuma           // timeVal
    );

    memset(noiseCols, 0, NUM_LEDS);
    fill_raw_noise16into8(
        noiseCols,
        NUM_LEDS,
        1,            // octaves
        33000,        //  x - don't mess with this one
        NUM_LEDS * 6, // scale
        timeCols);

    CRGBPalette16 runPal = CRGBPalette16(col[0], col[1], col[2], col[3]);

    for (int x = 0; x < kMatrixWidth; x++)
    {
        for (int y = 0; y < kMatrixHeight; y++)
        {

            leds[XY(x, y)] =
                ColorFromPalette(runPal,
                                 noiseCols[(y * kMatrixWidth) + x],
                                 brighten8_raw(map8(noiseData[x][y], 0, CurrentBri)));
            // noiseData[x][y],
            // brighten8_raw(map8(noiseCols[(y * kMatrixWidth) + x], 0, CurrentBri)));
        }
    }
}