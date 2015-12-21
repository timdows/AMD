#ifndef H_SOFTBUTTON
#define H_SOFTBUTTON

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class SoftButton{
private:
  int row;
  int column;
  int ledstripNumber;

  unsigned long previousMillis;

  bool shouldFlash;
  int flashNumber;
  bool flashLedStatus; //On or off;

  int currentRed;
  int currentGreen;
  int currentBlue;

  int veraIdRed;
  int veraIdGreen;
  int veraIdBlue;

  bool changeVeraRed;
  bool changeVeraGreen;
  bool changeVeraBlue;
  
  Adafruit_NeoPixel *ledstrip;

  void SetColorToStrip();
  void SetStripWhite();
  void SendCommandToVera(int id, int level);
public:
  SoftButton(int row, int column, int ledstripNumber, Adafruit_NeoPixel *ledstrip, int veraIdRed, int veraIdGreen, int veraIdBlue);
  void Update();
  void FlashButton();
  int GetRow();
  void SetColor(int red, int green, int blue);
  int GetRed();
  int GetGreen();
  int GetBlue();
};

#endif
