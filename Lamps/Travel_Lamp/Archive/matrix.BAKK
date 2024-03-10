uint8_t mtx(uint8_t x, uint8_t y)
{

    // any out of bounds address maps to the first hidden pixel
    if ((x >= kMatrixWidth) || (y >= kMatrixHeight))
    {
        return (NUM_LEDS);
    }

    uint8_t i;

    if (coil == true)
    {
        i = (y * kMatrixWidth) + x;
    }

    else
    {
        if (ser_col == true)
        {
            // Even columns, counting from top to bottom
            if (x % 2 == 0)
            {
                i = x * kMatrixHeight + y;
            }
            // Odd columns, counting from bottom to top
            else
            {
                i = x * kMatrixHeight + (kMatrixHeight - 1 - y);
            }
        }
        // otherwise we operate on rows (Y values)
        else
        {
            // Even rows, counting from left to right
            if (y % 2 == 0)
            {
                i = y * kMatrixWidth + x;
            }
            // Odd rows, counting from right to left
            else
            {
                i = y * kMatrixWidth + (kMatrixWidth - 1 - x);
            }
        }
    }

    // Optionally invert the index
    if (flip == true)
    {
        i = NUM_LEDS - 1 - i;
    }

    return i;
}