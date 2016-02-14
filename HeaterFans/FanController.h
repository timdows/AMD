// FanController.h

#ifndef _FANCONTROLLER_h
#define _FANCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <MySensor.h>
#include "Const.h"

class FanController
{
 protected:
	 MySensor *mySensorGw;

	 int childId;
	 int pin;

 public:
	 void Init(MySensor *mySensorGw, int childId, int pin);
	 void ProcessMessage(const MyMessage &message);
};

#endif

