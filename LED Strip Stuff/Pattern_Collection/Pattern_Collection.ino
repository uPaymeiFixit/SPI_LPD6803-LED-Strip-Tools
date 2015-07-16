////////////////////////////// SETUP AND CONST //////////////////////////////

// SETUP:
//    PIN GND -> GND
//    PIN 13  -> CLK
//    PIN 11  -> DAT

#include <FastSPI_LED.h>
#define NUM_LEDS 17
#define PIN 4
#define PI 3.14159

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

  Serial.begin(9600);
}

////////////////////////////// END SETUP AND CONST //////////////////////////////


void loop()
{
  //soundRainbowLoop()
  //faded(readSound());
  //rainbowLoop(readSound());
  rainbowLoop(40, true);
  //rainbow();
  //chase();
  //testFade();
  //jonsPattern();
  //setLEDs(255,0,0);
  //strobe(4);
}

////////////////////////////// PATTERNS //////////////////////////////

void soundRainbowLoop()
{
  int level = readSound();
  if (level > 100)
    rainbowLoop(level, false);
  else
    rainbowLoop(level, true);
}


int fades = 0;
void faded(int brightness)
{
  if (brightness > 150)
    fades = 256;

  fades--;
  setLEDs(fades,fades,fades);
}

int readSound()
{
  return map(analogRead(0), 0,1023,0,255);
}

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

  if (Serial.available() == 2)
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

void strobe(int delay_time)
{
  setLEDs(255,255,255);
  FastSPI_LED.show();
  delay(delay_time);

  clearLEDs();
  FastSPI_LED.show();
  delay(delay_time);
}

void jonsPattern()
{
  int dly = 100;
  byte rgb1[3];
  byte rgb2[3];
  hsvToRgb(random(360)/360.0,1,1,rgb1);
  hsvToRgb(random(360)/360.0,1,1,rgb2);


  for(int i = 0; i <= NUM_LEDS/2-1; i++)
  {
    clearLEDs();
    setLED(i, rgb1[0], rgb1[1], rgb1[2]);
    setLED((NUM_LEDS-1)-i, rgb2[0], rgb2[1], rgb2[2]);
    FastSPI_LED.show();
    delay(dly);
  }
  for(int i = NUM_LEDS/2; i >= 0; i--)
  {
    setLED(i, (rgb1[0]+rgb2[0])/2, (rgb1[1]+rgb2[1])/2, (rgb1[2]+rgb2[2])/2);
    setLED((NUM_LEDS-1)-i, (rgb1[0]+rgb2[0])/2, (rgb1[1]+rgb2[1])/2, (rgb1[2]+rgb2[2])/2);
    FastSPI_LED.show();
    delay(dly);
  }
}

// If you lay the strip in a circle, a color wheel will rotate
int rainbowLoop_lastStopped = 0;
void rainbowLoop(int _delay, bool _clockwise)
{
      for(int i = 0; i < NUM_LEDS; i++)
      {
        byte rgb[3];
        float hue = ((i+abs(rainbowLoop_lastStopped))%NUM_LEDS+0.0)/NUM_LEDS;
        hsvToRgb(hue,1,1,rgb);
        setLED(i, rgb[0], rgb[1], rgb[2]);
      }
    FastSPI_LED.show();
    delay(_delay);

    if (_clockwise)
      rainbowLoop_lastStopped++;
    else
      rainbowLoop_lastStopped--;
}


// Fades the whole strip through all the colors
void rainbow()
{
  for(int i = 1; i < 500; i++)
  {
    byte rgb[3];
    hsvToRgb(i/500.0,1,1,rgb);
    setLEDs(rgb[0], rgb[1], rgb[2]);

    delay(50);
    FastSPI_LED.show();
  }
}

// Sets the strip to a solid color
void setLEDs(int r, int g, int b)
{
  clearLEDs();
  for(int i = 0; i < NUM_LEDS; i++) {
    setLED(i, r, g, b);
  }
  FastSPI_LED.show();
}

// Chooses a random color, then chases that led with a faded trail behind it
void chase() {
  int r;
  int g;
  int b;
  if (false)
  {
    byte rgb[3];
    hsvToRgb(random(360)/360,1,1,rgb);
    r = rgb[0];
    g = rgb[1];
    b = rgb[2];
  }
  else
  {
    r = random(256);
    g = random(256);
    b = random(256);
  }

  clearLEDs();
  for(int i = 0; i < NUM_LEDS; i++)
  {
    for(int j = 0; j < NUM_LEDS; j++)
    {
      if (i > j-1)
      {
        if ( ((r/(j+1)) < 10) || ((g/(j+1)) < 10) || ((b/(j+1)) < 10) )
        {
          setLED(i-j, 0,0,0);
        }
        else {
          setLED(i-j, r/(j+1), g/(j+1), b/(j+1));
        }
      }
    }
    FastSPI_LED.show();
    delay(3000);
  }
}

// Chooses a random LED and fades it on and off
void testFade()
{
  int rnd = random(NUM_LEDS);

  clearLEDs();

  for(int x = 0; x < 801; x++)
  {
    int y = 127.5*(cos(x/127.5+PI)+1);
    setLED(rnd, y,y,y);
    delay(1);
    FastSPI_LED.show();
  }

}

////////////////////////////// END PATTERNS //////////////////////////////

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
