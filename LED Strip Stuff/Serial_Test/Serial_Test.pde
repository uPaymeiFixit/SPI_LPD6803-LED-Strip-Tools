////////////////////////////// SETUP AND CONST //////////////////////////////

// SETUP: 
//    PIN GND -> GND
//    PIN 13  -> CLK
//    PIN 11  -> DAT

#include <FastSPI_LED.h>
#define NUM_LEDS 17
//#define PIN 4
#define PI 3.14159

struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
struct CRGB *leds;

void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);

//  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 

  Serial.begin(9600);
}

////////////////////////////// END SETUP AND CONST //////////////////////////////


void loop()
{

}

////////////////////////////// PATTERNS //////////////////////////////

// Lets you serially set an on and off delay time for the @index led
// example input "0005, 0100" for 5ms on 100ms off
int incomingByte;
int onTime=10, offTime=10;
void setFrequency(int index)
{
    if (Serial.available() == 9)
    {
        onTime = Serial.read()*1000;
        onTime += Serial.read()*100;
        onTime += Serial.read()*10;
        onTime += Serial.read();
        onTime -= 53328;
        onTime /= 10;
        Serial.read();
        offTime = Serial.read()*1000;
        offTime += Serial.read()*100;
        offTime += Serial.read()*10;
        offTime += Serial.read();
        offTime -= 53328;
        offTime /= 10;
        Serial.print(onTime); Serial.println(offTime);
    }
    setLED(index,255,255,255);
    FastSPI_LED.show();
    delay(onTime);

    clearLEDs();
    FastSPI_LED.show();
    delay(offTime);

}

// Receives via serial the index of an LED to turn on
void manual()
{
  delay(16); // Lets the serial buffer catch up
  
  if (Serial.available() == 1)
  {
    clearLEDs();
    incomingByte = Serial.read()*10;
    incomingByte += Serial.read() - 528; // char '0'*10+'0' is 528
    setLED(incomingByte, 255,255,255);
    FastSPI_LED.show(); // Turns LEDs on
    Serial.println(incomingByte);

  }
  else if (Serial.available() == 1)
  {
    clearLEDs();
    incomingByte = Serial.read()-48; // char '0' is 48
    setLED(incomingByte, 255,255,255);
    FastSPI_LED.show(); // Turns LEDs on
    Serial.println(incomingByte);
  }
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