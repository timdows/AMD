#include "Dallas.h"

void DallasClass::Init(MySensor *mySensorGw, int childId, int pin)
{
	this->mySensorGw = mySensorGw;
	this->childId = childId;
	this->pin = pin;
	
	this->oneWire = OneWire(this->pin);
	this->sensors = DallasTemperature(&this->oneWire);

	this->sensors.begin();
}

void DallasClass::ReadSensor()
{
	this->sensors.requestTemperatures();
	float temperature = static_cast<float>(static_cast<int>(sensors.getTempCByIndex(0) * 10.)) / 10.;
	this->mySensorGw->send(MyMessage(this->childId, V_TEMP).set(temperature, 1));
}


