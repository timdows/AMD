// 
// 
// 

#include "Bounce.h"

void Bounce::Init(int pin)
{
	this->pin = pin;
	pinMode(this->pin, OUTPUT);
	digitalWrite(this->pin, INPUT_PULLUP);
}

bool Bounce::Update()
{
	unsigned long currentMillis = millis();

	if (currentMillis - this->previousMillis >= this->updateDelay) {
		this->previousMillis = currentMillis;  // Remember the time

		int sensorVal = digitalRead(this->pin);
		if (sensorVal == this->readStatus) {
			if (++this->currentRead == this->readsToChange) {
				this->publicStatus = sensorVal;
				return true;
			}
		}
		else {
			this->readStatus = sensorVal;
			this->currentRead = 0;
		}
	}

	return false;
}

bool Bounce::Status()
{
	return this->publicStatus;
}
