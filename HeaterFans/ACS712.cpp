#include "ACS712.h"

//Constructor
void ACS712::Init(MySensor *mySensorGw, int childId, int pin) {
	this->mySensorGw = mySensorGw;
	this->childId = childId;
	this->pin = pin;
}

void ACS712::CheckPowerUsage() {
	unsigned long currentMillis = millis();

	if (currentMillis - this->previousMillis >= this->intervalDelay || this->forceUpdate) {
		this->previousMillis = currentMillis;  // Remember the time
		this->forceUpdate = false;

		int rawValue = analogRead(this->pin);
		//Serial.print("Raw Value = "); // shows pre-scaled value 
		//Serial.print(rawValue);

		//double voltageA = (rawValue / 1023.0);
		//int voltage = voltageA * 5000;
		//float voltage = (rawValue * (5.0 / 1023.0)) * 1000;
		int voltage = (rawValue * (5.0 / 1023.0)) * 1000;
		//Serial.print("\tVoltage ");
		//Serial.print(voltage);

		int calcValue = abs(this->ACSoffset - voltage);
		//Serial.print("\tcalcValue "); // shows pre-scaled value 
		//Serial.print(calcValue); // the '3' after voltage allows you to display 3 digits after decimal point

		double amps = calcValue / this->mVperAmp;
		//Serial.print("\tamps "); // shows pre-scaled value 
		//Serial.print(amps, 3); // the '3' after voltage allows you to display 3 digits after decimal point

		int watt = amps * 12;

		this->mySensorGw->send(MyMessage(this->childId, V_WATT).set(watt));
	}
}

void ACS712::ForceUpdate() {
	this->forceUpdate = true;
}
