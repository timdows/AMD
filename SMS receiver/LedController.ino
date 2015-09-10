#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma

uint8_t dataPin  = 23;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 25;    // Green wire on Adafruit Pixels

Adafruit_WS2801 strip = Adafruit_WS2801(2, dataPin, clockPin);

void ledSetup(){
  strip.begin();
  strip.show();
  
  strip.setPixelColor(0, Color(200, 0, 0));
  strip.setPixelColor(1, Color(200, 0, 0));
  strip.show();
}

void SetWifiLedToColor(byte r, byte g, byte b){
  strip.setPixelColor(0, Color(r, g, b));
  strip.show();
}

void SetSimLedToColor(byte r, byte g, byte b){
  strip.setPixelColor(1, Color(r, g, b));
  strip.show();
}

uint32_t Color(byte r, byte g, byte b){
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
