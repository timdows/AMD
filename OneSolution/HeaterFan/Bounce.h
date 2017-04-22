// Bounce.h

#ifndef _BOUNCE_h
#define _BOUNCE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Bounce {
private:
	int pin;

	unsigned long previousMillis;
	int updateDelay = 100;
	int readsToChange = 3;
	int currentRead = 0;

	bool readStatus = false;
	bool publicStatus = false;

public:
	void Init(int pin);
	bool Update();
	bool Status();
};


#endif

