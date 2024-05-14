// Struct to hold info on color ranges in order to request certain palette types
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
    
//     char *paletteType;
// };


// CRGB pulsingColor(uint8_t hue, uint8_t hue_rng, uint8_t sat_min, uint8_t sat_max, uint8_t bri_min, uint8_t bri_max)
// {
//   if(init_run == false) {
//     // vari_rnd = random(1000);
//     int vari_rnd = random(1000);
//     Serial.println(vari_rnd);
//     return vari_rnd;
//     init_run = true;
//   }

//   uint16_t rnd_h = inoise16(millis()+vari_rnd);
//   uint16_t rnd_s = inoise16(millis());
//   // uint8_t rnd_s = random(10);
//   // uint8_t rnd_b = random(10);
//   //Serial.println(rnd_h);
//   uint8_t mapped_hue = map8(inoise8(millis()/10), hue-hue_rng, hue+hue_rng);
//   uint8_t mapped_sat = map8(inoise8(millis()/10), sat_min, sat_max);
//   uint8_t mapped_bri = map8(inoise8((millis()/10)+vari_rnd), bri_min, bri_max);

//   // CRGB color = CHSV(mapped_hue, mapped_sat, mapped_bri);
//   CRGB color = CHSV(hue, 255, mapped_bri);
//   return color;
// }




// class ColorPerlin {
//   CRGBPalette16 palette;
//   CRGB leds[NUM_LEDS];

//   uint8_t hue_min, hue_max, sat_min, sat_max, bri_min, bri_max;
//   uint8_t hue_min_2, hue_max_2, sat_min_2, sat_max_2, bri_min_2, bri_max_2;
//   uint8_t hue_min_3, hue_max_3, sat_min_3, sat_max_3, bri_min_3, bri_max_3;
//   uint8_t hue_min_4, hue_max_4, sat_min_4, sat_max_4, bri_min_4, bri_max_4;

//   uint16_t hue_speed, hue_speed_2, hue_speed_3, hue_speed_4;
//   uint16_t sat_speed, sat_speed_2, sat_speed_3, sat_speed_4;
//   uint16_t bri_speed, bri_speed_2, bri_speed_3, bri_speed_4;

// public:
//   ColorPerlin(uint8_t h_min, uint8_t h_max, uint8_t s_min, uint8_t s_max, uint8_t b_min, uint8_t b_max) {

//     h_speed = random16(256);
//     s_speed = random16(256);
//     b_speed = random16(256);
//   }

//   void update() {
//     fill_palette(leds, NUM_LEDS, 0, 255, palette, 255, LINEARBLEND);

//     for (int i = 0; i < NUM_LEDS; i++) {
//       uint8_t hue = map(noise16(h_speed), 0, 65535, h_min, hue_max);
//       uint8_t sat = map(noise16(s_speed), 0, 65535, s_min, sat_max);
//       uint8_t bri = map(noise16(b_speed), 0, 65535, b_min, bri_max);


//       leds[i] = CHSV(hue, sat, bri);
//       h_speed += PERLIN_SPEED_1;
//       s_speed += PERLIN_SPEED_1;
//       b_speed += PERLIN_SPEED_1;
//     }

//     FastLED.show();
//     delay(50);
//   }
// };





// void loop() {
//   ColorPerlin cp(30, 50, 150, 255, 150, 255);
  
//   while (true) {
//     cp.update();
//   }
// }
