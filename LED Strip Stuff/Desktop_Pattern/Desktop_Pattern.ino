// SETUP: 
//    PIN GND -> GND
//    PIN 13  -> CLK
//    PIN 11  -> DAT

#include <FastSPI_LED.h>
#define NUM_LEDS 50
#define PIN 4

struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
struct CRGB *leds;

void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);

  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
}

/*
Indexes and layout for desktop:

  0-2 right
  3-11 5" bay (front right)
  12 is the break on the GPU
  13-14 front left
  15-17 left
  18 left rear

  19-20 off

  21-26 curtain to ground

  27 back right
  28-29 right
  30-31 front right
  32-33 front left
  34-36 left
  37-40 right
  41-43 left
*/

////////////////////////////// END SETUP AND CONST //////////////////////////////


void loop() {
  setLED( 0, 255,255,255);
  setLED( 1, 255,255,255);
  setLED( 2, 255,255,255);
  setLED( 3, 100,100,255);
  setLED( 4, 100,100,255);
  setLED( 5, 100,100,255);
  setLED( 6, 100,100,255);
  setLED( 7, 100,100,255);
  setLED( 8, 100,100,255);
  setLED( 9, 100,100,255);
  setLED(10, 100,100,255);
  setLED(11, 100,100,255);
  setLED(12, 0,255,80);
  setLED(13, 0,255,80);
  setLED(14, 0,255,80);
  setLED(15, 0,255,80);
  setLED(16, 0,255,80);
  setLED(17, 0,255,80);
  setLED(18, 0,255,80);
  
  setLED(19, 0,0,0);
  setLED(20, 0,0,0);

  setLED(21, 255, 60,180);
  setLED(22, 255, 60,180);
  setLED(23, 255, 60,180);
  setLED(24, 255, 60,180); // back
  setLED(25, 255, 60,180);
  setLED(26, 255, 60,180);

  setLED(27,   0, 80,255);
  setLED(28,   0, 80,255); // right
  setLED(29,   0, 80,255);
  setLED(30,   0, 80,255);
  setLED(31,   0, 80,255);

  setLED(32,   0,255, 80);
  setLED(33,   0,255, 80);
  setLED(34,   0,255, 80); // left
  setLED(35,   0,255, 80);
  setLED(36,   0,255, 80);

  setLED(37,   0, 80,255);
  setLED(38,   0, 80,255);
  setLED(39,   0, 80,255); // right
  setLED(40,   0, 80,255);

  setLED(41,   0,255, 80);
  setLED(42,   0,255, 80); // left
  setLED(43,   0,255, 80);

  setLED(44, 255,255,255);
  setLED(45, 255,255,255);
  setLED(46, 255,255,255);
  setLED(47, 255,255,255); // center
  setLED(48, 255,255,255);
  setLED(49, 255,255,255);


  FastSPI_LED.show();
  delay(3);

  memset(leds, 0, NUM_LEDS*3); // Sets all LEDs off

  FastSPI_LED.show();
  delay(3);
}

// @param index int 0-49
// @param red, green, blue int 0-255
void setLED(int index, int red, int green, int blue)
{
      leds[index].r = red;
      leds[index].g = green;
      leds[index].b = blue;
}

////////////////////////////// END TOOLS //////////////////////////////
