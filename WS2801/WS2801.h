#ifndef H_WS2811
#define H_WS2811

#define NUMBER_OF_SCENES 4
#define NUM_LEDS 62

#include "Arduino.h"
#include <MySensor.h>
#include "Adafruit_NeoPixel.h"

#define LED_PIN 6

class WS2811{
private:
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);
//  Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, LED_PIN, NEO_RGB + NEO_KHZ800);
  boolean sceneStatus[NUMBER_OF_SCENES];// = { 0, 0, 0, 0, 0 };
  
  //RGB values
  int redRGB = 0;
  int greenRGB = 0;
  int blueRGB = 0;
  
  //states
  int currentColorWipe = 0;
  
  int currentKnightRider = 0;
  int delayKnightRider = 20;
  
  int currentRainbowCycle = 0;
  int delayRainbowCycle = 10;
  
  int currentTheaterChase = 0;
  int delayTheaterChase = 10;
  
  int delayDisco = 60;
  
  boolean currentFullRGB = false;
  boolean allScenesOff = true;
  
  int intervalDelay = 20;
  unsigned long previousMillis;
  
  MySensor *mySensorGw;
  
  void ColorWipe(uint32_t color);
  void KnightRider();
  void RainbowCycle();
  void Disco();
  void FullRGBColor(boolean state);
  uint32_t Wheel(byte WheelPos);
  void PrintDouble(double val, byte precision);
  
public:
  WS2811(MySensor *mySensorGw);
  //WS2811(int numberOfScenes);
  void Update();
  void ProcessScene(int sensor, int toValue);
  void SetRGBValue(const MyMessage &message);
};

#endif // H_WS2811

