#ifndef H_ACS712
#define H_ACS712

#define MY_DEBUG
#define MY_RADIO_NRF24

#include "Arduino.h"
#include "MySensorsCore.h"

class ACS712 {
private:
	//Mysensor child ID's
	uint8_t childId;
	uint8_t pin;

	unsigned long intervalDelay = 30000;
	unsigned long previousMillis;

	int mVperAmp = 185; // use 185 for the 5A and 100 for 20A Module and 66 for 30A Module
	int ACSoffset = 2531;

	float sumOfMeasurements;
	int readsCompleted;
	float lastCompletedSum;

public:
	ACS712(uint8_t childId, uint8_t pin);
	void CheckPowerUsage();
};

#endif // H_ACS712

