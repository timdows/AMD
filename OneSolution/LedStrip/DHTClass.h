// DHTClass.h

#ifndef _DHTClass_h
#define _DHTClass_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "DHT.h"
#include "C:\Users\Timdows\Documents\Arduino\libraries\MySensors\core\MySensorsCore.h"

class DHTClass
{
protected:
	DHT dht;

	int childIdTemp;
	int childIdHum;
	int pin;

	unsigned long intervalDelay = 60000;
	unsigned long previousMillis;

	void ReadSensor();

public:
	void Init(int childIdTemp, int childIdHum, int pin);
	void Update();
};

#endif
