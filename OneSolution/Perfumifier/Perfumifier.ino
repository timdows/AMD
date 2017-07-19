// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

// Set the NodeID
#define MY_NODE_ID 20

#include <MySensors.h>
#include "Adafruit_Si7021.h"
#include "Relay.h"
#include "external/MyMessage.h"

#define CHILD_TEMP 0
#define CHILD_HUM 1
#define CHILD_FAN 2
#define CHILD_PERFUMIFIER 3
#define CHILD_TIMER 4

Adafruit_Si7021 sensor = Adafruit_Si7021();
Relay relayPerfumifier(3, CHILD_PERFUMIFIER);
Relay relayFan(2, CHILD_FAN);
unsigned long previousMillis;
unsigned long timerPreviousMillis;
bool isTimerOn;

void setup()
{
	previousMillis = 0;
  isTimerOn = false;
	sensor.begin();
}

void presentation()
{
	present(CHILD_TEMP, S_TEMP);
  present(CHILD_HUM, S_HUM);
  present(CHILD_FAN, S_LIGHT);
  present(CHILD_PERFUMIFIER, S_LIGHT);
  present(CHILD_TIMER, S_LIGHT);

	//Send the sensor node sketch version information to the gateway
	sendSketchInfo("Perfumifier", "1.0");
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

 if (isTimerOn && currentMillis - timerPreviousMillis >= 60 * 1000) {
  Serial.println("Timer over");
  isTimerOn = false;
  pushPerfumifierButton();
  relayFan.SetOff(true);
 }
}


void receive(const MyMessage &message) {
  /*Serial.println(message.data);
  Serial.println(message.type);
  Serial.println(message.sensor);*/

  // We only expect one type of message from controller. But we better check anyway.
  if (message.type == V_LIGHT) {

    if (message.sensor == CHILD_FAN) {
      if (atoi(message.data) == 0) {
        relayFan.SetOff(true);
      }
      else {
        relayFan.SetOn(true);
      }
    }
    else if (message.sensor == CHILD_PERFUMIFIER) {
      pushPerfumifierButton();
    }
    // Check if a timer should be set
    else if (message.sensor == CHILD_TIMER) {
      Serial.println("Timer started");
      timerPreviousMillis = millis();
      isTimerOn = true;
      pushPerfumifierButton();
      relayFan.SetOn(true);
    }
  }
}

void pushPerfumifierButton() {
  // The board needs a short push
  relayPerfumifier.SetOn(false);
  delay(500);
  relayPerfumifier.SetOff(true);
}

