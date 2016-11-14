#include "FanController.h"

void FanController::Init(int childId, int pin)
{
	this->childId = childId;
	this->pin = pin;

	pinMode(this->pin, OUTPUT);
}

void FanController::SetOff()
{
	digitalWrite(this->pin, LOW);
	send(MyMessage(this->childId, V_LIGHT).set(0));
}

void FanController::SetOn()
{
	digitalWrite(this->pin, HIGH);
	send(MyMessage(this->childId, V_LIGHT).set(1));
}

void FanController::ProcessMessage(const MyMessage &message)
{
	int requestedLevel = atoi(message.data);
	
	Serial.print("Pin: ");
	Serial.print(this->pin);
	Serial.print(" requestedLevel: ");
	Serial.println(requestedLevel);

	//analogWrite(this->pin, (int)(requestedLevel / 100. * 255));

	digitalWrite(this->pin, message.getBool() ? HIGH : LOW);
	//this->mySensorGw->send(MyMessage(this->childId, V_LIGHT).set(message.getBool() ? 1 : 0));
}

