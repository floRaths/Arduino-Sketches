
uint8_t paletteIndex, switchPalette;

colorType colorType[4];
palette pal;
//scaleLimits lumScales, colScales;
scales scales;

void paletteSelection()
{
    switch (switchPalette)
    {
    case 0: // monochrome
    {
        Serial.println("monochrome");
        pal.col[0] = {hueA, 10, 155, 255, 155, 255};
        pal.col[1] = {hueA, 10, 155, 255, 155, 255};
        pal.col[2] = {hueA, 10, 155, 255, 155, 255};
        pal.col[3] = {hueA, 10, 155, 255, 155, 255};
    }
    break;

    case 1: // duotone
    {
        Serial.println("duotone");
        pal.col[0] = {hueA, 10, 155, 255, 155, 255};
        pal.col[1] = {hueA, 10, 155, 255, 155, 255};
        pal.col[2] = {hueA, 10, 155, 255, 155, 255};
        pal.col[3] = {hueA, 10, 155, 255, 155, 255};
    }
    break;

    case 2: // pastel
    {
        Serial.println("pastel");
        pal.col[0] = {hueA, 10, 155, 255, 155, 255};
        pal.col[1] = {hueA, 10, 155, 255, 155, 255};
        pal.col[2] = {hueA, 10, 155, 255, 155, 255};
        pal.col[3] = {hueA, 10, 155, 255, 155, 255};
    }
    break;
    }
}
