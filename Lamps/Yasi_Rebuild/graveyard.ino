// arrays for holding colors and their values
// uint8_t hue[4]; // used for selectng new palettes
// uint8_t sat[4];
// uint8_t bri[4];

// randomly creates a new 4-color palette based on two main hues
// the general logic of random selection is used for hue, sat, bri for each color
// void buildPalette()
// {
//     // coin = random(10);
//     // if ((coin % 2) == 0)
//     // {
//     //     hue[0] = random(base_hue1 - range, base_hue1 + range); // pick hue of first color with allowance of 'range'
//     // }

//     hue[0] = random(base_hue1 - range, base_hue1 + range); // pick hue of first color with allowance of 'range'
//     sat[0] = random(200, 255);                             // pick saturation for first color
//     bri[0] = random(240, 255);                             // same logic as for hues and sat above

//     hue[1] = random(base_hue2 - range, base_hue2 + range); // pick hue of second color with allowance of 'range'
//     sat[1] = random(200, 255);                             // pick saturation for second color
//     bri[1] = random(240, 255);

//     hue[2] = hue[0] + random(0, 20);
//     sat[2] = random(120, 255);       // sat[0] + random(0, 20); // same logic as for hues above
//     bri[2] = bri[0] - random(0, 30);

//     hue[3] = hue[1] + random(0, 20); // pick hue of fourth color, which can be up to 20-clicks different from second
//     sat[3] = random(120, 255); // sat[1] + random(0, 20); // same logic as for hues above
//     bri[3] = bri[1] - random(0, 30);

//     // after selecting our 12 color components, we store the results in the palette
//     for (uint8_t i = 0; i < 4; i++)
//     {
//         pal[i] = CHSV(hue[i], sat[i], bri[i]);
//     }
// }

// // to increase contrast, we need to know the highest value in the current frame
// if (lumNoise[x][y] > maxLumValue)
// {
//     maxLumValue = lumNoise[x][y]; // Update the maximum value to the highest found in the frame
// }
// if (colNoise[x][y] > maxColValue)
// {
//     maxColValue = colNoise[x][y]; // Update the maximum value to the highest found in the frame
// }

// // expand the value accordingly to the determined max value (+ 5 to reduce clipping).
// lumNoise[x][y] = map(lumNoise[x][y], 0, maxLumValue + 5, 0, 255);
// colNoise[x][y] = map(colNoise[x][y], 0, maxColValue + 5, 0, 255);

// uint8_t noiseCols[NUM_LEDS];



// memset(noiseCols, 0, NUM_LEDS);
// fill_raw_noise16into8(
//     noiseCols,
//     NUM_LEDS,
//     1,            // octaves
//     33000,        //  x - don't mess with this one
//     NUM_LEDS * 6, // scale
//     timeCols);
