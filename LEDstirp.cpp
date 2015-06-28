#include <FastSPI_LED.h>

#define NUM_LEDS 50

// SETUP: 
//    PIN GND -> GND
//    PIN 13  -> CLK
//    PIN 11  -> DAT

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

#define PIN 4
#define PI 3.1415926535897932384626433832795

int rnd;


void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);

  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 


}

bool on = false;

void loop() {

  //solidColor(0,150,255);
  //delay(1);
  rainbow();

  //chase();
  //testFade();
}

void rainbow()
{
  for(int i = 1; i < 500; i++)
  {
    byte rgb[3];
    hsvToRgb(i/500.0,1,1,rgb);
    solidColor(rgb[0], rgb[1], rgb[2]);

    delay(500);
  }
}

void solidColor(int r, int g, int b)
{
  if (r < 10 && r != 0) r = 10;
  if (g < 10 && g != 0) g = 10;
  if (b < 10 && b != 0) b = 10;

  memset(leds, 0, NUM_LEDS * 3);
  for(int i = 0; i < 50; i++) {
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  FastSPI_LED.show();
}

void randomMoves() {
  int randDelay = random(100);
  int r = random(255);
  int g = random(255);
  int b = random(255);

  int position = 0;
  int move = random(50-position);

}

void chase() {
  int r = random(256);
  int g = random(256);
  int b = random(256);


  memset(leds, 0, NUM_LEDS * 3);
  for(int i = 0; i < NUM_LEDS; i++)
  {
    for(int j = 0; j < NUM_LEDS; j++)
    {
      if (i > j-1)
      {
        if ( ((r/((j+1)*(j+1))) < 10) || ((g/((j+1)*(j+1))) < 10) || ((b/((j+1)*(j+1))) < 10) )
        {
          leds[i-j].r = 0;
          leds[i-j].g = 0;
          leds[i-j].b = 0;
        }
        else {
          leds[i-j].r = r/((j+1)*(j+1));
          leds[i-j].g = g/((j+1)*(j+1));
          leds[i-j].b = b/((j+1)*(j+1));
        }
      }
    }

    FastSPI_LED.show();
    delay(25-i/3);
  }
}

void testFade()
{
  rnd = random(50);

  memset(leds, 0, NUM_LEDS * 3);

  // int r = random(255);
  // int g = random(255);
  // int b = random(255);
  for(int x = 0; x < 801; x++)
  {
    int y = 127.5*(cos(x/127.5+PI)+1);
    leds[rnd].r = y;
    leds[rnd].g = y;
    leds[rnd].b = y;
    FastSPI_LED.show();
    delay(1);
  }

}


void hsvToRgb(double h, double s, double v, byte rgb[]) {
    double r, g, b;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}