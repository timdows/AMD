#include "DHTClass.h"

void DHTClass::ReadSensor()
{
	// Remove this?
	delay(dht.getMinimumSamplingPeriod());

	float temperature = dht.getTemperature();
	if (isnan(temperature)) {
		Serial.println("Failed reading temperature from DHT");
	}
	else {
		this->mySensorGw->send(MyMessage(this->childIdTemp, V_TEMP).set(temperature, 1));
		Serial.print("Temp: ");
		Serial.println(temperature);
	}

	float humidity = dht.getHumidity();
	if (isnan(humidity)) {
		Serial.println("Failed reading humidity from DHT");
	}
	else {
		this->mySensorGw->send(MyMessage(this->childIdHum, V_HUM).set(humidity, 1));
		Serial.print("Humidity: ");
		Serial.println(humidity);
	}
}

void DHTClass::Init(MySensor *mySensorGw, int childIdTemp, int childIdHum, int pin)
{
	this->mySensorGw = mySensorGw;
	this->childIdTemp = childIdTemp;
	this->childIdHum = childIdHum;
	this->pin = pin;

	this->dht.setup(this->pin, DHT::DHT11);
}