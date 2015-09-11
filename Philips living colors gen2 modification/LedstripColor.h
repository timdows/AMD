#ifndef H_LEDSTRIPCOLOR
#define H_LEDSTRIPCOLOR

#include "Arduino.h"
#include <MySensor.h> 

class LedstripColor{
private:
  int mySensorChildId;
  int ledPin;
  int intervalDelay;
  
  int currentLedLevel;
  int desiredLedLevel;
  int lastLedLevel;
  unsigned long previousMillis;
  boolean stateChanged;
  
  MySensor gw;
  MyMessage dimmerMsg;
  MyMessage lightMsg;
  
public:
  LedstripColor(MySensor mySensorGw, int pin, int childId, int intDelay);
  int GetChildId();
  void SetDesiredLedLevel(int ledLevel);
  void ChangeLevelInPercent(double percent);
  int GetCurrentLedLevel();
  void RestoreLastLedLevel();
  void Update();
  void ForceGwUpdate();
};

#endif // H_LEDSTRIPCOLOR
