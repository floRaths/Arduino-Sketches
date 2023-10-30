

uint8_t data = 0;
uint8_t scaledData = 0;
uint8_t maxLumValue = 0;

void makeNoise()
{
    memset(lumNoise, 0, NUM_LEDS);
    fill_raw_2dnoise16into8(
        (uint8_t *)lumNoise,
        kMatrixHeight,   // width
        kMatrixHeight,   // height
        1,               // octaves
        0,               // x
        50000,            // scalex
        0,               // y
        50000,            // scaley
        millis() * hurry // timeVal
    );

    // To increase contrast, update the maximum value if the input is higher
    for (int x = 0; x < kMatrixWidth; x++)
    {
        for (int y = 0; y < kMatrixHeight; y++)
        {
            if (lumNoise[x][y] > maxValue)
            {
                maxValue = lumNoise[x][y]; // Update the maximum value to the highest found in the frame
            }
        }
    }

    scaledData = qadd8(lumNoise[4][10], scale8(lumNoise[4][10], maxValue));

    if (dataSmoothing)
    {   
        uint8_t olddata = data;
        uint16_t newdata = (uint16_t)olddata * (256 - 128) + (uint16_t)scaledData * 128;
        data = newdata / 256; // Convert back to uint8_t
    }
    else
    {
        data = scaledData;
    }

    Serial.print(maxValue);
    Serial.print(",");
    Serial.print(lumNoise[4][10]);
    Serial.print(",");
    Serial.print(data);
    Serial.print(",");
    Serial.println(scaledData + 5);

    //maxValue = 0;
}

// void smoothAndMap()
// {
//     // if dataSmoothing is requested, a rolling average is calculated between last and current frame
//     if (dataSmoothing > 0)
//     {
//         uint8_t oldLumData = lumNoise[x][y];
//         uint8_t newLumData = scale8(oldLumData, 256 - dataSmoothing) +
//                              scale8(lumNoise[x][y], dataSmoothing);
//         lumNoise[x][y] = newLumData;
//     }

//     // to increase contrast, we need to know the highest value in the current frame
//     if (lumNoise[x][y] > maxValue)
//     {
//         maxValue = lumNoise[x][y]; // Update the maximum value to the highest found in the frame
//     }

//     // expand the value accordingly to the determined max value (+ 5 to reduce clipping).
//     lumNoise[x][y] = map(lumNoise[x][y], 0, maxValue + 5, 0, 255);
// }

// Define a function to expand data and update the maximum value
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