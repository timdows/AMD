#ifndef H_RELAY
#define H_RELAY

#include "Arduino.h"
#include "external/MySensorsCore.h"

class Relay{
private:
  int pin;
  int childId;
  
public:
  Relay(int pin, int childId);
  void SetOn();
  void SetOff();
  boolean GetState();
};

#endif // H_RELAY

