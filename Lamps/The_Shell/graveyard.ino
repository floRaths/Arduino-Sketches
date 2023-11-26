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

// void rollColors(int ramp1_speed)
// {
//     if (rolling == true)
//     {
//         Serial.println();
//         Serial.println("Rolling Colors");

//         // Increment cycleCount for the next iteration
//         cycleCount0 = (cycleCount0 + 1) % 4;
//         cycleCount1 = (cycleCount1 + 1) % 4;
//         cycleCount2 = (cycleCount2 + 1) % 4;
//         cycleCount3 = (cycleCount3 + 1) % 4;

//         // start smoothly blending colors
//         palRamp1.go(255, ramp1_speed, QUINTIC_IN);
//         rolling = false;
//     }
//     // Iterate through the elements of the arrays
//     col[0] = nblend(col[0], pal[cycleCount0], palRamp1.update());
//     col[1] = nblend(col[1], pal[cycleCount1], palRamp1.update());
//     col[2] = nblend(col[2], pal[cycleCount2], palRamp1.update());
//     col[3] = nblend(col[3], pal[cycleCount3], palRamp1.update());

//     // after the slower ramp is done, we terminate the color blending and re-set the used ramps
//     if (palRamp1.isFinished() == 1)
//     {
//         palRamp1.go(0, 0, NONE);
//     }
// }

// void newHues(uint8_t exclude_min, uint8_t exclude_max, uint8_t min_diff)
// {
//     do
//     {
//         base_hue1 = random(0, 255 + 1); // Generate a random integer for the first variable
//         do
//         {
//             base_hue2 = random(0, 255 + 1);              // Generate a random integer for the second variable
//         } while (abs(base_hue2 - base_hue1) < min_diff); // Ensure a difference of at least 20
//         do
//         {
//             base_hue3 = random(0, 255 + 1);
//         } while ((abs(base_hue3 - base_hue2) < min_diff) &&
//                  (abs(base_hue3 - base_hue1) < min_diff));
//     } while (
//         (base_hue1 >= exclude_min && base_hue1 <= exclude_max) || // Check exclusion range for the first variable
//         (base_hue2 >= exclude_min && base_hue2 <= exclude_max)    // Check exclusion range for the second variable
//     );

//     if (reporting)
//     {
//         Serial.println();
//         Serial.print("Hue1: ");
//         Serial.print(base_hue1);
//         Serial.print(", Hue2: ");
//         Serial.print(base_hue2);
//         Serial.print(", delta = ");
//         Serial.println(abs(base_hue1 - base_hue2));
//     }
// }