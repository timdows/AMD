// Switch.h

#ifndef _SWITCH_h
#define _SWITCH_h

#include "Arduino.h"
#include <MySensor.h>

class Switch
{
 private:
	MySensor *mySensorGw;
	int childId;
	int pin;
	bool state;

 public:
	Switch(MySensor *mySensorGw, int childId, int pin);
	void SetOff(bool updateGateway);
	void SetOn(bool updateGateway);
	bool GetState();
};


#endif

