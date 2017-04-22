// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Enabled repeater feature for this node
#define MY_REPEATER_FEATURE

#include <MySensors.h>
#include "Adafruit_Si7021.h"
#include "external/MyMessage.h"

#define CHILD_TEMP 0
#define CHILD_HUM 1

Adafruit_Si7021 sensor = Adafruit_Si7021();
unsigned long previousMillis;

void setup()
{
	previousMillis = 0;
	sensor.begin();
}

void presentation()
{
	present(CHILD_TEMP, S_TEMP);
	present(CHILD_HUM, S_HUM);

	//Send the sensor node sketch version information to the gateway
	sendSketchInfo("Repeater with temp", "1.0");
}

void loop()
{
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= 30 * 1000)
	{
		previousMillis = currentMillis;

		Serial.print("Humidity:    ");
		Serial.print(sensor.readHumidity(), 2);
		Serial.print("\tTemperature: ");
		Serial.println(sensor.readTemperature(), 2);
		
		send(MyMessage(CHILD_TEMP, V_TEMP).set(sensor.readTemperature(), 2));
		send(MyMessage(CHILD_HUM, V_HUM).set(sensor.readHumidity(), 2));
	}
}
