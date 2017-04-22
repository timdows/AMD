// ButtonWithLed.h

#ifndef _BUTTONWITHLED_h
#define _BUTTONWITHLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Bounce.h"
#include "MySensorsCore.h"

class ButtonWithLed {
private:
	int childId;
	int buttonPin;
	int buttonLedPin;

	Bounce button;

	bool shouldChange = false;
	unsigned long previousMillis;
	unsigned long dimmerDelay = 10;
	bool goOnOff = true; // True is on, false is off
	int currentBrightness = 0;

public:
	void Init(int childId, int buttonPin, int buttonLedPin);
	bool Update();
	bool Status();
	void SetStatus(bool newStatus);
};

#endif

