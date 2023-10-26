
void triggerBlend(int ramp1_speed, int ramp2_speed, bool randomize)
{
        Serial.println();
        Serial.println("Shuffling Colors");
        buildPalette(randomize); // creating a new palette

        new_colors = true;   // allows blending of old colors to new colors

        // start smoothly blending colors
        palRamp1.go(255, ramp1_speed, QUINTIC_IN);
        palRamp2.go(255, ramp2_speed, QUINTIC_IN);
}

// when a new palette is requested, the old colors are blended towards that new state
void blendColors()
{
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
        if (palRamp2.isFinished() == 1 && palette_changed == true)
        {
            palette_changed = false;
            briRamp.go(stored_bri, 500, LINEAR);
            changeScales(4000);
        }
    }
}