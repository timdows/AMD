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

		Serial.print(this->lastCompletedSum);
		Serial.print(" sum --- ");

		float ampsScale = 5.0 / 1024 / this->mVperAmp; //5/1024 = 0.00488  // Sensitivity = 0.185V on 5A
		Serial.print(ampsScale, 4);
		Serial.print(" ampsScale --- ");
		
		// 510 for ws28xx plafond
		// 514 for heater fans
		// 510 for entree mirror
		float offset = ampsScale * 510; //this->lastCompletedSum when not connected (508 in tests)
		Serial.print(offset, 4);
		Serial.print(" offset --- ");

		float amps = ampsScale * this->lastCompletedSum - offset;
		
		// Make float positive
		amps = fabsf(amps);

		Serial.print(amps, 4);
		Serial.print(" amps --- ");
		
		// Make sure the volts are the volts from the input power and not the arduino
		float watts = amps * 5.0;

		Serial.print(watts, 4);
		Serial.println(" watts");

		send(MyMessage(this->childId, V_WATT).set(watts, 2));
	}
}
