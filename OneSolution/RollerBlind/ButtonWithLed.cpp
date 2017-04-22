// 
// 
// 

#include "ButtonWithLed.h"

void ButtonWithLed::Init(int childId, int buttonPin, int buttonLedPin)
{
	this->childId = childId;
	this->buttonPin = buttonPin;
	this->buttonLedPin = buttonLedPin;

	this->button.Init(this->buttonPin);
	pinMode(this->buttonLedPin, OUTPUT);
}

bool ButtonWithLed::Update()
{
	// returns true if button has changed
	if (this->button.Update() && !this->shouldChange) {
		this->shouldChange = true;

		this->goOnOff = !this->goOnOff;
		send(MyMessage(this->childId, V_LIGHT).set(this->goOnOff ? 1 : 0));

		return true;
	}

	unsigned long currentMillis = millis();

	if (currentMillis - this->previousMillis >= this->dimmerDelay && this->shouldChange) {
		this->previousMillis = currentMillis;  // Remember the time

		if (this->goOnOff) {
			if (this->currentBrightness++ >= 100) {
				this->currentBrightness = 100;
				this->shouldChange = false;
				return false;
			}
		}
		else {
			if (this->currentBrightness-- <= 0) {
				this->currentBrightness = 0;
				this->shouldChange = false;
				return false;
			}
		}

		int value = (int)(this->currentBrightness / 100. * 255);
		analogWrite(this->buttonLedPin, value);
	}

	//if (this->shouldChange) {
	//	this->shouldChange = false;
	//	if (this->goOnOff) {
	//		analogWrite(this->buttonLedPin, 255);
	//	}
	//	else {
	//		analogWrite(this->buttonLedPin, 0);
	//	}
	//}

	return false;
}

bool ButtonWithLed::Status()
{
	return this->goOnOff;
}

void ButtonWithLed::SetStatus(bool newStatus)
{
	this->goOnOff = newStatus;
	this->shouldChange = true;
	send(MyMessage(this->childId, V_LIGHT).set(this->goOnOff ? 1 : 0));
}
