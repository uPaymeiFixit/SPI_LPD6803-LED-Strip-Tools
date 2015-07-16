/***
    bitlash-demo.pde

    Bitlash is a tiny language interpreter that provides a serial port shell environment
    for bit banging and hardware hacking.

    This is an example demonstrating how to use the Bitlash2 library for Arduino 0015.

    Bitlash lives at: http://bitlash.net
    The author can be reached at: bill@bitlash.net

    Copyright (C) 2008-2012 Bill Roy

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:
    
    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
***/

// This is the simplest bitlash integration.

#include "bitlash.h"
#include <FastSPI_LED.h>

#define NUM_LEDS 17
#define PIN 4
#define PI 3.14159

struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
struct CRGB *leds;

void setup(void) {

    // initialize bitlash and set primary serial port baud
    // print startup banner and run the startup macro
    initBitlash(57600);

  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);

  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
}

void loop(void) {
    runBitlash();
}

////////////////////////////// TOOLS //////////////////////////////

// @param h,s,v float from 0 to 1
// @param rgb -> byte rgb[3];
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

// @param index int 0-49
// @param red, green, blue int 0-255
void setLED(int index, int red, int green, int blue)
{
    // The LEDs don't work for values in range (0,10),
    // so this keeps them out of that range.
    // TO-DO, write a PWM funciton to fix this
    if (red   < 10 && red   != 0) red   = 10;
    if (green < 10 && green != 0) green = 10;
    if (blue  < 10 && blue  != 0) blue  = 10;

    leds[index].r = red;
    leds[index].g = green;
    leds[index].b = blue;
}

// Sets all LEDs to off
void clearLEDs()
{
    memset(leds, 0, NUM_LEDS * 3);
}

////////////////////////////// END TOOLS //////////////////////////////