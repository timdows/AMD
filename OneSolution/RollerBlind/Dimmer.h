// Dimmer.h

#ifndef _DIMMER_h
#define _DIMMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Bounce.h"
#include "MySensorsCore.h"

class Dimmer {
private:
	int ledPin;
	int childId;
	int buttonPin;
	int buttonLedPin;

	Bounce lightButton;

	bool shouldChange = false;
	unsigned long previousMillis;
	unsigned long dimmerDelay = 10;
	bool goOnOff = false; // True is on, false is off
	int currentBrightness = 0;

public:
	void Init(int ledPin, int childId, int buttonPin, int buttonLedPin);
	void Update();
	void SetStatus(bool value);
};


#endif

