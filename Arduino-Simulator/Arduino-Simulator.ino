#include <FastLED.h>
#include <Ramp.h>
#include <OneButton.h>

#define LED_PIN 5
#define BTN_PIN 7

//OneButton btn = OneButton(BTN_PIN, true, true);

const uint8_t MatrixX = 16;
const uint8_t MatrixY = 16;
#define NUM_LEDS MatrixX * MatrixY
#define LAST_LED NUM_LEDS
CRGB leds[NUM_LEDS];

const bool coil = true;
const bool flip = false;
const bool serpentine = false;
const bool prototyping = false;
const int  xOffset = -4;

uint8_t hurry = 4;
//const char *paletteNames[] = {"monochrome", "duotone", "tricolore", "pastel", "pastelAccent", "static"};
const char *paletteNames[] = {"monochrome", "duotone"};
const int  *brightnessVals[] = {255, 128, 86};

#include "auxFnss/auxFnss.h"

palette pllt;
scales  scls;
stripRange strp;

bool init_run = false;


//#include "buttons/buttons.h"

// ################## SETUP ####################
void setup()
{
  delay(100); // startup safety delay  
  Serial.begin(115200);
  Serial.println("Hello Arduino\n");
  randomSeed(analogRead(0));

  FastLED.addLeds < WS2812B, LED_PIN, GRB > (leds, NUM_LEDS);
  
}



// ################## LOOP ####################
void loop()
{
  //float noise = inoise8(millis())/25.0;
  //float sclnoise = map(noise, 0, 255, 1, 85) / 10.0;
  //Serial.println(noise);
  
  uint8_t mappedh1 = map8(inoise8(millis()/2), 30, 50);
  uint8_t mappedh2 = map8(inoise8(millis()/3), 150, 170);

  
  // uint8_t mappedValue = map(inoise8(millis()/7), 0, 255, 50, 150);
  // uint8_t mappedValue = map(inoise8(millis()/7), 0, 255, 50, 150);
  
  // CRGB color1 = CHSV(mappedh1, 255, 255);
  //CRGB color2 = CHSV(170, 255, 255);
  CRGB color1 = pulsingColor(40, 10, 100, 255, 100, 255);
  CRGB color2 = pulsingColor(140, 10, 100, 255, 100, 255);
  // CRGB color2 = pulsingColor(140, 10, 100, 255, 100, 255);
  
  CRGBPalette16 runPal = CRGBPalette16(color1, color2);
  
  fill_palette(leds, NUM_LEDS, 250, 1, runPal, 255, LINEARBLEND);
  
  FastLED.show();
}

CRGB pulsingColor(uint8_t hue, uint8_t hue_rng, uint8_t sat_min, uint8_t sat_max, uint8_t bri_min, uint8_t bri_max)
{
  if(init_run == false) {
    // vari_rnd = random(1000);
    int vari_rnd = random(1000);
    Serial.println(vari_rnd);
    return vari_rnd;
    init_run = true;
  }

  uint16_t rnd_h = inoise16(millis()+vari_rnd);
  uint16_t rnd_s = inoise16(millis());
  // uint8_t rnd_s = random(10);
  // uint8_t rnd_b = random(10);
  //Serial.println(rnd_h);
  uint8_t mapped_hue = map8(inoise8(millis()/10), hue-hue_rng, hue+hue_rng);
  uint8_t mapped_sat = map8(inoise8(millis()/10), sat_min, sat_max);
  uint8_t mapped_bri = map8(inoise8((millis()/10)+vari_rnd), bri_min, bri_max);

  // CRGB color = CHSV(mapped_hue, mapped_sat, mapped_bri);
  CRGB color = CHSV(hue, 255, mapped_bri);
  return color;
}