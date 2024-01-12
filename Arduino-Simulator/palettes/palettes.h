
void paletteSelection(palette &palette, const String &paletteType, bool reporting = false)
{
    if (executed == false)
    {
        if (paletteType == "monochrome")
        {
            palette.col[0] = {palette.hueA, 10, 55, 255, 155, 255};
            palette.col[1] = {palette.hueA, 10, 155, 255, 155, 255};
            palette.col[2] = {palette.hueB, 10, 155, 255, 155, 255};
            palette.col[3] = {palette.hueB, 10, 55, 255, 155, 255};
        }
        else if (paletteType == "duotone")
        {
            palette.col[0] = {palette.hueA, 10, 55, 255, 155, 255};
            palette.col[1] = {palette.hueA, 10, 155, 255, 155, 255};
            palette.col[2] = {palette.hueB, 10, 155, 255, 155, 255};
            palette.col[3] = {palette.hueB, 10, 55, 255, 155, 255};
        }
        else if (paletteType == "pastel")
        {
            palette.col[0] = {palette.hueA, 10, 5, 5, 155, 255};
            palette.col[1] = {palette.hueA, 10, 5, 5, 155, 255};
            palette.col[2] = {palette.hueA, 10, 5, 5, 155, 255};
            palette.col[3] = {palette.hueA, 10, 5, 5, 155, 255};
        }
        else if (paletteType == "static")
        {
            palette.col[0] = {palette.hueA, 0, 255, 255, 255, 255};
            palette.col[1] = {palette.hueA, 0, 255, 255, 255, 255};
            palette.col[2] = {palette.hueA, 0, 255, 255, 255, 255};
            palette.col[3] = {palette.hueA, 0, 255, 255, 255, 255};
        }
        else // "Unknown palette type"
        {   
            palette.col[0] = {palette.hueA, 10, 1, 255, 1, 255};
            palette.col[1] = {palette.hueB, 10, 1, 255, 1, 255};
            palette.col[2] = {palette.hueC, 10, 1, 255, 1, 255};
            palette.col[3] = {palette.hueD, 10, 1, 255, 1, 255};
        }
        executed = true;
        
        if (reporting)
        {
            Serial.print(paletteType);
            Serial.println(" palette");
        }
    }
}