
uint8_t paletteIndex, switchPalette;

colorType colorType[4];
scaleLimits lumScales, colScales;

void paletteSelection()
{
    switch (switchPalette)
    {
    case 0: // monochrome
    {
        colorType[0] = {hueA, 10, 155, 255, 155, 255};
        colorType[1] = {hueA, 10, 155, 255, 155, 255};
        colorType[2] = {hueA, 10, 155, 255, 155, 255};
        colorType[3] = {hueA, 10, 155, 255, 155, 255};
    }
    break;

    case 1: // duotone
    {
        colorType[0] = {hueA, 10, 155, 255, 155, 255};
        colorType[1] = {hueA, 10, 155, 255, 155, 255};
        colorType[2] = {hueA, 10, 155, 255, 155, 255};
        colorType[3] = {hueA, 10, 155, 255, 155, 255};
    }
    break;

    case 2: // pastel
    {
        colorType[0] = {hueA, 10, 155, 255, 155, 255};
        colorType[1] = {hueA, 10, 155, 255, 155, 255};
        colorType[2] = {hueA, 10, 155, 255, 155, 255};
        colorType[3] = {hueA, 10, 155, 255, 155, 255};
    }
    break;
    }
}
