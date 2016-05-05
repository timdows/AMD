// 
// 
// 

#include "Switch.h"

Switch::Switch(MySensor *mySensorGw, int childId, int pin)
{
	pinMode(pin, OUTPUT);

	this->mySensorGw = mySensorGw;
	this->childId = childId;
	this->pin = pin;

	this->state = false;
}

void Switch::SetOff(bool updateGateway)
{
	digitalWrite(this->pin, LOW);
	if (updateGateway)
	{
		this->mySensorGw->send(MyMessage(this->childId, V_LIGHT).set(0));
	}
	this->state = false;
}

void Switch::SetOn(bool updateGateway)
{
	digitalWrite(this->pin, HIGH);
	if (updateGateway)
	{
		this->mySensorGw->send(MyMessage(this->childId, V_LIGHT).set(1));
	}
	this->state = true;
}

bool Switch::GetState()
{
	return this->state;
}


