/*
* ACS712 class
*/
#include "ACS712.h"

ACS712::ACS712(uint8_t childId, uint8_t pin)
{
	this->childId = childId;
	this->pin = pin;
	this->sumOfMeasurements = 0;
	this->readsCompleted = 0;
	this->lastCompletedSum = 0;
}

void ACS712::CheckPowerUsage() {
	unsigned long currentMillis = millis();

	// Add the raw value to the sum
	this->sumOfMeasurements += analogRead(this->pin);

	// Get the average of 100 values
	if (++this->readsCompleted == 100) {
		this->readsCompleted = 0;
		this->lastCompletedSum = this->sumOfMeasurements / 100;
		this->sumOfMeasurements = 0;
	}

	if (currentMillis - this->previousMillis >= this->intervalDelay) {
		this->previousMillis = currentMillis;  // Remember the time

		//Serial.print(this->lastCompletedSum);
		//Serial.println("Sum");

		float ampsScale = 0.00488 / 0.185; //5/1024 = 0.00488  // Sensitivity = 185mV
		float amps = ampsScale * this->lastCompletedSum - 13.40; // 13.40 = (0.00488/0.185)*this->lastCompletedSum when not connected (508 in tests)
		float volts = amps * 5.0;

		send(MyMessage(this->childId, V_WATT).set(volts, 2));
	}
}
