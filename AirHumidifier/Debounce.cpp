/*
* Debounce class
*/
#include "Debounce.h"

//Constructor
Debounce::Debounce(MySensor *mySensorGw, int child, int pin) {
	this->pin_switch = pin;
	this->mySensorGw = mySensorGw;
	this->debounceMsg = MyMessage(child, V_TRIPPED);
	this->debouncer = Bounce();

	this->currentValue = 0;
	this->hasChanged = false;

	// Setup the button / switch
	pinMode(this->pin_switch, INPUT);
	// Activate internal pull-up
	digitalWrite(this->pin_switch, HIGH);

	// After setting up the button, setup debouncer
	this->debouncer.attach(this->pin_switch);
	this->debouncer.interval(5);

}

void Debounce::Update() {
	this->debouncer.update();
	// Get the update value
	int value = this->debouncer.read();

	if (value != this->currentValue) {
		// Send in the new value
		this->mySensorGw->send(this->debounceMsg.set(value == HIGH ? 1 : 0));
		this->currentValue = value;
		this->hasChanged = true;
	}
}

boolean Debounce::HasChanged() {
	return this->hasChanged;
}

int Debounce::GetValue() {
	this->hasChanged = false;
	return this->currentValue;
}