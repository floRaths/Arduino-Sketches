int maxLumValue = 0; // Assume the first element is the maximum

// uint8_t testDataBefore = 128;
// CRGB colorz = CRGB::Green;

// CRGB pulsingColor(float speed, uint8_t hue, uint8_t hue_rng, uint8_t sat_min, uint8_t sat_max, uint8_t bri_min, uint8_t bri_max)
// {
//   //if(init_run == false) {
//     // vari_rnd = random(1000);
//     //int vari_rnd = 900;
//     //Serial.println(vari_rnd);
//     //return vari_rnd;
//     //init_run = true;
//   //}

//   //uint16_t rnd_h = inoise16(millis()+vari_rnd);
//   //uint16_t rnd_s = inoise16(millis());
//   // uint8_t rnd_s = random(10);
//   // uint8_t rnd_b = random(10);
//   //Serial.println(rnd_h);
//   uint8_t mapped_hue = map8(inoise8(millis()/(speed*2.0)), hue-hue_rng, hue+hue_rng);
//   uint8_t mapped_sat = map8(inoise8(millis()/(speed*3.0)), sat_min, sat_max);
//   uint8_t mapped_bri = map8(inoise8(millis()/(speed*5.0)), bri_min, bri_max);

//   // CRGB color = CHSV(mapped_hue, mapped_sat, mapped_bri);
//   CRGB color = CHSV(mapped_hue, mapped_sat, mapped_bri);
//   return color;
// }


void makeNoise(palette pllt, scales &scales, int motionSpeed, bool dataSmoothing)
{
    uint8_t lumNoise[MatrixX][MatrixY]; 
    uint8_t lumData[MatrixX][MatrixY];
    uint8_t scaled_lumData[MatrixX][MatrixY];

    uint8_t colNoise[MatrixX][MatrixY];
    uint8_t colData[MatrixX][MatrixY];


    memset(lumNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)lumNoise,
        MatrixY,                  // width
        MatrixY,                  // height
        1,                        // octaves
        scales.xyVals[0],         // x
        scales.lumRampX.update(), // scalex
        scales.xyVals[1],         // y
        scales.lumRampY.update(), // scaley
        millis() * motionSpeed    // timeVal
    );

    memset(colNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)colNoise,
        MatrixY,                  // width
        MatrixY,                  // height
        1,                        // octaves
        scales.xyVals[2],         // x
        scales.colRampX.update(), // scalex
        scales.xyVals[3],         // y
        scales.colRampY.update(), // scalex
        millis() * (motionSpeed / 4)    // timeVal
    );

    CRGBPalette16 runPal = CRGBPalette16(pllt.runCol[0], pllt.runCol[1], pllt.runCol[2], pllt.runCol[3]);
    // CRGB col1 = pulsingColor(2.5, 30, 10, 100, 255, 100, 255);
    // CRGB col2 = pulsingColor(3.7, 150, 10, 100, 150, 100, 255);
    // CRGBPalette16 runPal = CRGBPalette16(col1, col1, col2, col2);

    for (int x = 0; x < MatrixX; x++)
    {
        for (int y = 0; y < MatrixY; y++)
        {            
            // find highest value in current frame and set as limit
            if (lumNoise[x][y] >= maxLumValue)
            {
                    maxLumValue = (lumNoise[x][y]);
            }

            // upscale noise values based on current limit
            scaled_lumData[x][y] = qadd8(lumNoise[x][y], scale8(lumNoise[x][y], maxLumValue));

            if (dataSmoothing)
            {
                uint8_t  old_lumData = lumData[x][y];
                uint16_t new_lumData = (uint16_t)old_lumData * (256 - 128) + (uint16_t)scaled_lumData[x][y] * 128;
                lumData[x][y] = new_lumData / 256; // Convert back to uint8_t

                uint8_t old_colData = colData[x][y];
                uint16_t new_colData = (uint16_t)old_colData * (256 - 128) + (uint16_t)colNoise[x][y] * 128;
                colData[x][y] = new_colData / 256; // Convert back to uint8_t
            }
            else
            {
                lumData[x][y] = scaled_lumData[x][y];
                colData[x][y] = colNoise[x][y];
            }

            leds[mtx(x, y, -4)] = ColorFromPalette(runPal,
                                                        // noiseCols[(y * MatrixX) + x], // when used with 1D colors
                                                        colData[x][y] + paletteIndex,
                                                        brighten8_lin(lumData[x][y]));

            // if (lumData[MatrixX / 2][MatrixY / 2] > testDataBefore)
            // { // Increasing
            //     leds[mtx(MatrixX / 2, MatrixY / 2)] = CRGB::Green;
            //     colorz = CRGB::Green;
            // }
            // else if (lumData[MatrixX / 2][MatrixY / 2] < testDataBefore)
            // { // Decreasing
            //     leds[mtx(MatrixX / 2, MatrixY / 2)] = CRGB::Red;
            //     colorz = CRGB::Red;
            // }

            // leds[mtx(MatrixX / 2, MatrixY / 2)] = colorz;

            // testDataBefore = lumData[MatrixX / 2][MatrixY / 2];
        }

    }
}