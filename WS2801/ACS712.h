#ifndef H_ACS712
#define H_ACS712

#include "Arduino.h"
#include <MySensor.h>

class ACS712{
private:
  MySensor *mySensorGw;
  
  unsigned long intervalDelay = 6000;
  unsigned long previousMillis;
  
  boolean forceUpdate = false;
  
  const int analogIn = 3;
  int mVperAmp = 185; // use 185 for the 5A and 100 for 20A Module and 66 for 30A Module
  int ACSoffset = 2500; 
  
public:
  ACS712(MySensor *mySensorGw);
  void CheckPowerUsage();
  void ForceUpdate();
};

#endif // H_ACS712

