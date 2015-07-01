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

void loop() {
  
    for (int i = 0; i < 8; i++)
    {
        setLED(i, 100, 100, 255);
    }
    for (int i = 8; i < 17; i++)
    {
        setLED(i, 0, 255, 80);
    }


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
