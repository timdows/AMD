#include "DHTClass.h"

void DHTClass::ReadSensor()
{
	float humidity = dht.readHumidity();
	float t = dht.readTemperature();

	// Compute heat index in Celsius (isFahreheit = false)
	float temperature = dht.computeHeatIndex(t, humidity, false);

	if (isnan(temperature)) {
		Serial.println("Failed reading temperature from DHT");
		send(MyMessage(this->childIdTemp, V_TEMP).set(0.0, 2));
	}
	else {
		send(MyMessage(this->childIdTemp, V_TEMP).set(temperature, 2));
		Serial.print("Temp: ");
		Serial.println(temperature);
	}

	if (isnan(humidity)) {
		Serial.println("Failed reading humidity from DHT");
		send(MyMessage(this->childIdHum, V_HUM).set(0.0, 0));
	}
	else {
		send(MyMessage(this->childIdHum, V_HUM).set(humidity, 0));
		Serial.print("Humidity: ");
		Serial.println(humidity);
	}
}

void DHTClass::Init(int childIdTemp, int childIdHum, int pin)
{
	this->childIdTemp = childIdTemp;
	this->childIdHum = childIdHum;
	this->pin = pin;

	this->dht = DHT(pin, DHT11);
	this->dht.begin();
	this->previousMillis = millis() - this->intervalDelay;
}

void DHTClass::Update()
{
	unsigned long currentMillis = millis();

	if (currentMillis - this->previousMillis >= this->intervalDelay) {
		this->previousMillis = currentMillis;  // Remember the time
		this->ReadSensor();
	}
}
