// DHTClass.h

#ifndef _DHTClass_h
#define _DHTClass_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <DHT.h>
#include <MySensor.h>
#include "Const.h"

class DHTClass
{
 protected:
	 DHT dht;
	 MySensor *mySensorGw;

	 int childIdTemp;
	 int childIdHum;
	 int pin;

 public:
	void Init(MySensor *mySensorGw, int childIdTemp, int childIdHum, int pin);
	void ReadSensor();
};

#endif

