// #include "auxFnss.h"

// struct colorType
// {
//     uint8_t hue;
//     uint8_t hueFluct;
//     uint8_t satMin;
//     uint8_t satMax;
//     uint8_t briMin;
//     uint8_t briMax;
// };

// struct palette
// {
//     uint8_t hueA, hueB, hueC, hueD;
//     colorType recipe[4];
    
//     CRGB newCol[4];
//     CRGB runCol[4];
//     CRGB oldCol[4];
    
//     String paletteType;
// };

// const colorType PROGMEM duotoneConfig[] = {
//     {100, 10, 55, 255, 100, 255},
//     {100, 10, 180, 255, 220, 255},
//     {150, 10, 180, 255, 220, 255},
//     {150, 10, 55, 255, 100, 255}
// };

// const colorType PROGMEM monochromeConfig[] = {
//     {100, 10, 100, 255, 155, 255},
//     {100, 10, 155, 255, 55, 255},
//     {100, 10, 155, 255, 55, 255},
//     {100, 10, 100, 255, 155, 255}
// };

// const colorType PROGMEM tricoloreConfig[] = {
//     {100, 20,  85, 255, 150, 255},
//     {100, 28, 155, 255, 150, 255},
//     {150, 27, 155, 255, 150, 255},
//     {200, 27, 155, 255, 150, 255}
// };

// const colorType PROGMEM pastelConfig[] = {
//     {100, 10, 102, 102, 100, 255},
//     {100, 10, 102, 102, 220, 255},
//     {150, 10, 102, 102, 220, 255},
//     {150, 10, 255, 255, 100, 255}
// };

// const colorType PROGMEM staticConfig[] = {
//     {100, 10,   0,   0, 255, 255}
// };

// const char * const paletteNames[] PROGMEM = {"duotone", "monochrome", "tricolore", "pastel", "static"};

// void updatePalette(palette &palette, String &paletteType, bool increment = false, bool replace_all = true, bool reporting = false)
// {
//     if (increment)
//     {
//         switchPalette = (switchPalette + 1) % (sizeof(paletteNames) / sizeof(paletteNames[0]));
//         paletteType = paletteNames[switchPalette];
//     }

//     uint8_t satu = 255;

//     if (paletteType == "duotone")
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             memcpy_P(&palette.recipe[i], &duotoneConfig[i], sizeof(colorType));
//         }
//     }
//     else if (paletteType == "monochrome")
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             memcpy_P(&palette.recipe[i], &monochromeConfig[i], sizeof(colorType));
//         }
//     }
//     else if (paletteType == "tricolore")
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             memcpy_P(&palette.recipe[i], &tricoloreConfig[i], sizeof(colorType));
//         }
//     }
//     else if (paletteType == "pastel")
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             memcpy_P(&palette.recipe[i], &pastelConfig[i], sizeof(colorType));
//         }
//     }
//     else if (paletteType == "static")
//     {
//         for (int i = 0; i < 4; i++)
//         {
//             memcpy_P(&palette.recipe[i], &staticConfig[0], sizeof(colorType));
//         }
//     }
//     else // "Unknown palette type"
//     {
//         palette.recipe[0] = {palette.hueA, 10, 1, 255, 1, 255};
//         palette.recipe[1] = {palette.hueB, 10, 1, 255, 1, 255};
//         palette.recipe[2] = {palette.hueC, 10, 1, 255, 1, 255};
//         palette.recipe[3] = {palette.hueD, 10, 1, 255, 1, 255};
//     }

//     if (reporting)
//     {
//         Serial.println();
//         if (increment)
//         {
//             Serial.print(">> switching to ");
//             Serial.print(paletteType);
//             Serial.println(" palette");
//         }
//         else
//         {
//             Serial.print(">> updating ");
//             Serial.print(paletteType);
//             Serial.println(" palette");
//         }
//     }

//     assemblePalette(palette, replace_all, reporting);
// }





// // #include "Arduino.h"

// // class Palettes
// // {
// // public:
// //   Palettes(){};
// //   bool runPattern();

// // private:
// //   uint16_t _plasmaTime = 0;
// //   struct colorType
// //   {
// //     uint8_t hue;
// //     uint8_t hueFluct;
// //     uint8_t satMin;
// //     uint8_t satMax;
// //     uint8_t briMin;
// //     uint8_t briMax;
// //   };
// // };

// // bool Palettes::runPattern() {

// //   return true;
// // }

// // void runPlasma(){
// //   bool isRunning = true;
// //   Palettes palettes = Palettes();
// //   while(isRunning) isRunning = palettes.runPattern();
// // }
