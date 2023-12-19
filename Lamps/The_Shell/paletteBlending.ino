uint8_t list[] = {0, 1, 2, 3};

void triggerRoll(int roll_speed)
{

    //randomize the order of the colors
    for (int i = 0; i < 4; ++i)
    {
        int r = random(i, 4);
        int temp = list[i];
        list[i] = list[r];
        list[r] = temp;
    }

    Serial.println();
    Serial.print("triggered roll with: ");
    Serial.print(list[0]);
    Serial.print(list[1]);
    Serial.print(list[2]);
    Serial.println(list[3]);

    for (int i = 0; i < 4; i++)
    {
        newCol2[i] = newCol[list[i]];
    }

    rolling = true;

    // start interpolation of blending function
    palRamp1.go(255, roll_speed * 0.65, LINEAR);
    palRamp2.go(255, roll_speed, LINEAR);
}

void rollColors()
{
    runCol[0] = blend(oldCol[0], newCol2[0], palRamp1.update());
    runCol[1] = blend(oldCol[1], newCol2[1], palRamp2.update());
    runCol[2] = blend(oldCol[2], newCol2[2], palRamp1.update());
    runCol[3] = blend(oldCol[3], newCol2[3], palRamp2.update());


    // after the slower ramp is done, we terminate the color blending and re-set
    if (palRamp2.isFinished() == 1 && rolling == true)
    {
        for (int i = 0; i < 4; i++)
        {
            tmpCol[i] = newCol2[i];
            newCol2[i] = oldCol[i];
            oldCol[i] = tmpCol[i];
        }

        palRamp1.go(0, 0, NONE);
        palRamp2.go(0, 0, NONE);
        
        rolling = false;
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