/* Hello Wokwi! */

#include "LiquidCrystal_I2C.h"
#include "FastLED.h"

const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 13;
boolean coil = true;
boolean flip = false;
boolean ser_col = true;
uint8_t hurry = 10;
uint8_t dataSmoothing = 120; // You can adjust this value

#define NUM_LEDS kMatrixWidth *kMatrixHeight
CRGB leds[NUM_LEDS];

LiquidCrystal_I2C lcd(0x27, 20, 4);

uint8_t lumNoise[kMatrixHeight][kMatrixHeight];

int currentMin = 50; // Initialize currentMin to the maximum possible integer value
int currentMax = 50; // Initialize currentMax to the minimum possible integer value

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Tello, Wokwi!");
}

void loop() {
  makeNoise();

  EVERY_N_MILLISECONDS(1000) {
    // Print the current minimum and maximum values
    Serial.print("Current Minimum: ");
    Serial.println(currentMin);
    Serial.print("Current Maximum: ");
    Serial.println(currentMax);
    Serial.println(leds[XY(4, 10)].getLuma());
  }
}

