#include <FastLED.h>

boolean coil = true;
boolean flip = false;
boolean ser_col = true;

#define LED_PIN 6
#define kMatrixWidth 15
#define kMatrixHeight 15
#define NUM_PARTICLES 35
#define NUM_LEDS kMatrixWidth *kMatrixHeight

CRGB leds[kMatrixWidth * kMatrixHeight];

//uint8_t flowField[kMatrixWidth][kMatrixHeight];

float xOff = 0.0;
float yOff = 0.0;
float noiseScale = 0.1;

struct Vector
{
  float x;
  float y;

  // Default constructor
  Vector() : x(0.0), y(0.0) {}

  // Parameterized constructor
  Vector(float x_, float y_) : x(x_), y(y_) {}

  float magnitude()
  {
    return sqrt(x * x + y * y);
  }

  void normalize()
  {
    float mag = magnitude();
    if (mag != 0)
    {
      x /= mag;
      y /= mag;
    }
  }
};

Vector flowField[kMatrixWidth][kMatrixHeight];

void generateFlowField()
{
  float xOff = 0.0;
  float yOff = 0.0;
  float noiseScale = 0.1;

  for (int x = 0; x < kMatrixWidth; x++)
  {
    for (int y = 0; y < kMatrixHeight; y++)
    {
      float angle = inoise8(xOff, yOff) * TWO_PI * 2.0;
      flowField[x][y] = {cos(angle), sin(angle)}; // Initialize using curly braces
      flowField[x][y].normalize();
      yOff += noiseScale;
    }
    xOff += noiseScale;
  }
}

class Particle
{
public:
  float x, y;
  Particle()
  {
    x = random(kMatrixWidth);
    y = random(kMatrixHeight);
  }

  void move()
  {
    int nearestX = int(x);
    int nearestY = int(y);

    // Move towards the closest vector in the flow field
    x += flowField[nearestX][nearestY].x;
    y += flowField[nearestX][nearestY].y;

    // Wrap around the edges
    x = fmod(x, kMatrixWidth);
    y = fmod(y, kMatrixHeight);
  }

  void display()
  {
    leds[mtx(x, y)] = CRGB::White;
  }
};

Particle particles[NUM_PARTICLES];

void updateFlowField()
{
  generateFlowField();
}

void updateParticles()
{
  for (int i = 0; i < NUM_PARTICLES; i++)
  {
    particles[i].move();
    particles[i].display();
  }
}

// ################## SETUP ####################
void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FastLED.addLeds<WS2812B, 5, GRB>(leds, NUM_LEDS);
}

void loop()
{
  FastLED.clear();

  updateFlowField();
  updateParticles();

  FastLED.show();
  delay(30);
}
