/*
* LedstripColor class
*/
#include "LedstripColor.h"

//Constructor
void LedstripColor::Init(int pin, int childId, int intDelay) {
	currentLedLevel = -1; //To make sure it always will update led on start
	stateChanged = false;
	lastLedLevel = 0;

	this->ledPin = pin;
	pinMode(this->ledPin, OUTPUT);
	this->mySensorChildId = childId;
	this->intervalDelay = intDelay;
}

int LedstripColor::GetChildId() {
	return mySensorChildId;
}

//From 0 - 100 only
void LedstripColor::SetDesiredLedLevel(int ledLevel) {
	lastLedLevel = currentLedLevel; //Not here as it will remember for all 3 the led colors a value??
	desiredLedLevel = ledLevel;
}

void LedstripColor::ChangeLevelInPercent(double percent) {
	lastLedLevel = currentLedLevel;
	desiredLedLevel = floor(desiredLedLevel * percent);
	//Debug
	/*Serial.print("Changing pin ");
	Serial.print(ledPin);
	Serial.print(" to ");
	Serial.println(desiredLedLevel);*/
}

int LedstripColor::GetCurrentLedLevel() {
	return currentLedLevel;
}

void LedstripColor::RestoreLastLedLevel() {
	desiredLedLevel = lastLedLevel;
}

void LedstripColor::Update() {
	unsigned long currentMillis = millis();

	//Debug
	/*
	Serial.print("Changing pin ");
	Serial.print(ledPin);
	Serial.print(" currentLedLevel ");
	Serial.print(currentLedLevel);
	Serial.print(" desiredLedLevel ");
	Serial.print(desiredLedLevel);
	Serial.print(" previousMillis ");
	Serial.print(previousMillis);
	Serial.print(" currentMillis ");
	Serial.println(currentMillis);*/

	if ((currentLedLevel != desiredLedLevel) && (currentMillis - previousMillis >= intervalDelay)) {
		stateChanged = true;
		previousMillis = currentMillis;  // Remember the time

		int delta = (desiredLedLevel - currentLedLevel) < 0 ? -1 : 1;
		currentLedLevel += delta;
		//Debug
		/*
		Serial.print("Changing pin ");
		Serial.print(ledPin);
		Serial.print(" to ");
		Serial.println(currentLedLevel);*/
		analogWrite(ledPin, (int)(currentLedLevel / 100. * 255));
	}
	else if (stateChanged && (currentMillis - previousMillis >= intervalDelay)) {
		stateChanged = false;
		previousMillis = currentMillis;  // Remember the time

										 // Inform the gateway of the current DimmableLED's SwitchPower1 and LoadLevelStatus value...
		send(MyMessage(this->mySensorChildId, V_LIGHT).set(currentLedLevel > 0 ? 1 : 0));
		send(MyMessage(this->mySensorChildId, V_DIMMER).set(currentLedLevel));
	}
}

void LedstripColor::ForceGwUpdate() {
	stateChanged = true;
}
