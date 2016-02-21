// Dallas.h

#ifndef _DALLAS_h
#define _DALLAS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <MySensor.h>
#include <DallasTemperature.h>
#include <OneWire.h>

class DallasClass
{
protected:
	MySensor *mySensorGw;

	int childId;
	int pin;

	OneWire oneWire;
	DallasTemperature sensors;

public:
	void Init(MySensor *mySensorGw, int childId, int pin);
	void ReadSensor();
};


#endif

