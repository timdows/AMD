// Dimmer.h

#ifndef _DIMMER_h
#define _DIMMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Bounce.h"
#include "C:\Users\Timdows\Documents\Arduino\libraries\MySensors\core\MySensorsCore.h"

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
	bool goOnOff = true; // True is on, false is off
	int currentBrightness = 0;
  
  bool wakeupLightStarted1 = false;
  bool wakeupLightStarted2 = false;
  bool wakeupLightStarted3 = false;
  bool wakeupLightStarted4 = false;
  bool wakeupLightStarted5 = false;
  bool wakeupLightStarted6 = false;
  bool wakeupLightStarted7 = false;
  bool wakeupLightStarted8 = false;
  bool wakeupLightStarted9 = false;
  unsigned long wakeupLightStartMillis;
  unsigned long wakeupLightDelay = 60000;

public:
	void Init(int ledPin, int childId, int buttonPin, int buttonLedPin);
	void Update();
	void SetStatus(bool value);
  void StartWakeupLight();
};


#endif
