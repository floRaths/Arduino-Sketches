// arrays for holding colors and their values
uint8_t hue[4]; // used for selectng new palettes
uint8_t sat[4];
uint8_t bri[4];

uint8_t satt = 120;

// randomly creates a new 4-color palette based on two main hues
// the general logic of random selection is used for hue, sat, bri for each color
void buildPalette()
{
    // coin = random(10);
    // if ((coin % 2) == 0)
    // {
    //     hue[0] = random(base_hue1 - range, base_hue1 + range); // pick hue of first color with allowance of 'range'
    // }

    // coin = random(10);
    // if ((coin % 2) == 0)
    // {
    //     hue[1] = random(base_hue2 - range, base_hue2 + range); // pick hue of second color with allowance of 'range'
    // }

    hue[0] = random(base_hue1 - range, base_hue1 + range); // pick hue of first color with allowance of 'range'
    hue[1] = random(base_hue2 - range, base_hue2 + range); // pick hue of second color with allowance of 'range'
    hue[2] = hue[0] + random(0, 20);
    hue[3] = hue[1] + random(0, 20); // pick hue of fourth color, which can be up to 20-clicks different from second

    sat[0] = random(200, 255);       // pick saturation for first color
    sat[1] = random(200, 255);       // pick saturation for second color
    sat[2] = random(120, 255);       // sat[0] + random(0, 20); // same logic as for hues above
    sat[3] = random(120, 255);       // sat[1] + random(0, 20); // same logic as for hues above

    bri[0] = random(240, 255); // same logic as for hues and sat above
    bri[1] = random(240, 255);
    bri[2] = bri[0] - random(0, 30);
    bri[3] = bri[1] - random(0, 30);

    // after selecting our 12 color components, we store the results in the palette
    for (uint8_t i = 0; i < 4; i++)
    {
        pal[i] = CHSV(hue[i], sat[i], bri[i]);
    }
}

// when a new palette is requested, the old colors are blended towards that new state
void blendColors(int ramp1_speed, int ramp2_speed)
{

    // grant_blend is false until paletteButton is pressed or after x seconds have passed
    if (grant_blend == true)
    {

        // creating a new palette
        buildPalette();

        grant_blend = false; // deactivate grant_blend
        new_colors  = true;   // allows blending of old colors to new colors

        // start smoothly blending colors
        palRamp1.go(255, ramp1_speed, QUINTIC_IN);
        palRamp2.go(255, ramp2_speed, QUINTIC_IN);
    }

    if (new_colors == true)
    {

        // with above smoothing active, the colors are blended to the new palette
        col[0] = nblend(col[0], pal[0], palRamp1.update());
        col[1] = nblend(col[1], pal[1], palRamp2.update());
        col[2] = nblend(col[2], pal[2], palRamp1.update());
        col[3] = nblend(col[3], pal[3], palRamp2.update());

        // after the slower ramp is done, we terminate the color blending and re-set the used ramps
        if (palRamp2.isFinished() == 1)
        {
            new_colors = false;
            palRamp1.go(0, 0, NONE);
            palRamp2.go(0, 0, NONE);
        }

        // After a palette change was called, this chunk finalizes the change and
        // returns the brightness back to where it was (because the palette change dips to black)
        if (palRamp2.isFinished() == 1 & palette_changed == true)
        {
            palette_changed == false;
            briRamp.go(stored_bri, 500, LINEAR);
            changeScales(4000);
        }
    }
}