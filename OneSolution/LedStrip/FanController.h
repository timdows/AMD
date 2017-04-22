// FanController.h

#ifndef _FANCONTROLLER_h
#define _FANCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "MySensorsCore.h"

class FanController
{
protected:
	int childId;
	int pin;

public:
	void Init(int childId, int pin);
	void SetOff();
	void SetOn();
	void ProcessMessage(const MyMessage &message);
};

#endif

