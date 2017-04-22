// 
// 
// 

#include "Dimmer.h"

void Dimmer::Init(int ledPin, int childId, int buttonPin, int buttonLedPin)
{
	this->ledPin = ledPin;
	this->childId = childId;
	this->buttonPin = buttonPin;
	this->buttonLedPin = buttonLedPin;

	this->lightButton.Init(this->buttonPin);
	pinMode(this->ledPin, OUTPUT);
	pinMode(this->buttonLedPin, OUTPUT);

	send(MyMessage(this->childId, V_LIGHT).set(0));
}

void Dimmer::Update()
{
	//Serial.println(this->shouldChange);

	// returns true if button has changed
	if (this->lightButton.Update() && !this->shouldChange) {
		this->shouldChange = true; 
		
		this->goOnOff = !this->goOnOff;
		/*Serial.print("New state");
		Serial.println(this->goOnOff);*/
	}

	unsigned long currentMillis = millis();

	if (currentMillis - this->previousMillis >= this->dimmerDelay && this->shouldChange) {
		this->previousMillis = currentMillis;  // Remember the time
		
		if (this->goOnOff) {
			if (this->currentBrightness++ >= 100) {
				this->currentBrightness = 100;
				this->shouldChange = false;
				return;
			}
		}
		else {
			if (this->currentBrightness-- <= 0) {
				this->currentBrightness = 0;
				this->shouldChange = false;
				return;
			}
		}

		/*Serial.print("currentBrightness ");
		Serial.println(this->currentBrightness);*/

		int value = (int)(this->currentBrightness / 100. * 255);

		analogWrite(this->ledPin, value);
		analogWrite(this->buttonLedPin, value);
	}
}

void Dimmer::SetStatus(bool value)
{
	this->shouldChange = true;
	this->goOnOff = value;
}
