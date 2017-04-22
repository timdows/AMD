// LedstripColor.h

#ifndef _LEDSTRIPCOLOR_h
#define _LEDSTRIPCOLOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MySensorsCore.h"


class LedstripColor {
private:
	int mySensorChildId;
	int ledPin;
	int intervalDelay;

	int currentLedLevel;
	int desiredLedLevel;
	int lastLedLevel;
	unsigned long previousMillis;
	boolean stateChanged;

public:
	void Init(int pin, int childId, int intDelay);
	int GetChildId();
	void SetDesiredLedLevel(int ledLevel);
	void ChangeLevelInPercent(double percent);
	int GetCurrentLedLevel();
	void RestoreLastLedLevel();
	void Update();
	void ForceGwUpdate();
};

#endif

