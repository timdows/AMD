#include "FanController.h"

void FanController::Init(MySensor *mySensorGw, int childId, int pin)
{
	this->mySensorGw = mySensorGw;
	this->childId = childId;
	this->pin = pin;

	pinMode(this->pin, OUTPUT);
}

void FanController::ProcessMessage(const MyMessage &message)
{
	digitalWrite(this->pin, message.getBool() ? HIGH : LOW);
	this->mySensorGw->send(MyMessage(this->childId, V_LIGHT).set(message.getBool() ? 1 : 0));
}

