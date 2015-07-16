// SETUP: 
//    PIN GND -> GND
//    PIN 13  -> CLK
//    PIN 11  -> DAT

#include <FastSPI_LED.h>
#define NUM_LEDS 17
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

int top[] = {2, 1, 0, 16};
int top_length = 4;
int front[] = {15, 14, 13, 12, 11};
int front_length = 5;
int bottom[] = {10, 9, 8, 7};
int bottom_length = 4;
int back[] = {6, 5, 4, 3};
int back_length = 4;

void loop() {
  
//    for (int i = 0; i < top_length; i++)
//    {
//        setLED(top[i], 100, 100, 255);
//    }
//    for (int i = 0; i < front_length; i++)
//    {
//        setLED(front[i], 0, 255, 80);
//    }
//    for (int i = 0; i < bottom_length; i++)
//    {
//        setLED(bottom[i], 100, 100, 255);
//    }
//    for (int i = 0; i < back_length; i++)
//    {
//        setLED(back[i], 0, 255, 80);
//    }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    setLED(i, 255,255,255);
  }


  FastSPI_LED.show();
  delay(4);

  memset(leds, 0, NUM_LEDS*3); // Sets all LEDs off

  FastSPI_LED.show();
  delay(4);
}

// @param index int 0-49
// @param red, green, blue int 0-255
void setLED(int index, int red, int green, int blue)
{
  if (index == 5 || index == 3 | index == 6) return;
    leds[index].r = red;
    leds[index].g = green;
    leds[index].b = blue;
}
