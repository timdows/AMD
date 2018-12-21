// 
// 
// 

#include "Bounce.h"

void Bounce::Init(int pin)
{
	this->pin = pin;
	pinMode(this->pin, INPUT_PULLUP);
}

bool Bounce::Update()
{
	unsigned long currentMillis = millis();

	if (currentMillis - this->previousMillis >= this->updateDelay) {
		this->previousMillis = currentMillis;  // Remember the time
		
		// Oopsie with NO vs NC
		int sensorVal = !digitalRead(this->pin);
		if (sensorVal == this->readStatus && sensorVal){
			Serial.print("In readstatus ");
			Serial.print(this->pin);
			Serial.print(" - ");
      Serial.print(this->currentRead);
      Serial.print(" - ");
      Serial.print(this->readsToChange);
      Serial.print(" - ");
			Serial.println(sensorVal);

			if (++this->currentRead == this->readsToChange) {
				this->publicStatus = sensorVal;
				Serial.println("In readstatus bounce true");
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
	// Oopsie while soldering, connected NC in stead of NO
	return !this->publicStatus;
}
