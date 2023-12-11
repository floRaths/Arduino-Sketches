uint8_t list[] = {0, 1, 2, 3};

void triggerRoll(int roll_speed)
{
    Serial.println();
    Serial.print("triggered roll with: ");
    Serial.print(list[0]); Serial.print(list[1]); Serial.print(list[2]); Serial.println(list[3]);

    // randomize the order of the colors
    for (int i = 0; i < 4; ++i)
    {
        int r = random(i, 4);
        int temp = list[i];
        list[i] = list[r];
        list[r] = temp;
    }

    // start interpolation of blending function
    palRamp1.go(255, roll_speed * 0.65, LINEAR);
    palRamp2.go(255, roll_speed, LINEAR);

    rolling = true;
}

void rollColors()
{
    // announce the great achievement
    if (rolling)
    {
        Serial.println("rolling ...");
        rolling = false;
    }

    // colors with odd index (1 & 3) are blended with palRamp1
    // colors with even index (0 & 2) are blended with palRamp2
    col[0] = nblend(col[0], pal[list[0]], palRamp1.update());
    col[1] = nblend(col[1], pal[list[1]], palRamp2.update());
    col[2] = nblend(col[2], pal[list[2]], palRamp1.update());
    col[3] = nblend(col[3], pal[list[3]], palRamp2.update());

    // for (int i = 0; i < 4; ++i)
    // {
    //     if (i % 2 == 0)
    //     {
    //         col[i] = nblend(col[i], pal[list[i]], palRamp2.update());
    //     }
    //     else
    //     {
    //         col[i] = nblend(col[i], pal[list[i]], palRamp1.update());
    //     }
    // }

    // after the slower ramp is done, we terminate the color blending and re-set
    if (palRamp2.isFinished() == 1)
    {
        palRamp1.go(0, 0, NONE);
        palRamp2.go(0, 0, NONE);
    }

    // when a palette change was called, the brightness dips down, so we need to
    // return the brightness back to where it was
    if (palRamp2.isFinished() == 1 && palette_changed == true)
    {
        palette_changed = false;
        briRamp.go(stored_bri, 1500, LINEAR);
        
        if (indexDrift == false) {
            paletteIndex = 0;
            Serial.println("resetting palIndex");
        }
    }
}