// ACS712.h

#ifndef _ACS712_h
#define _ACS712_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <MySensor.h>

class ACS712 {
private:
	MySensor *mySensorGw;

	int childId;
	int pin;

	unsigned long intervalDelay = 6000;
	unsigned long previousMillis;

	boolean forceUpdate = false;

	float mVperAmp = 185; // use 185 for the 5A and 100 for 20A Module and 66 for 30A Module
	int ACSoffset = 2550;

public:
	void Init(MySensor *mySensorGw, int childId, int pin);
	void CheckPowerUsage();
	void ForceUpdate();
};

#endif

