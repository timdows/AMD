#define MY_DEBUG
#define MY_RADIO_NRF24


#include <DHT.h>
#include "DHTClass.h"
#include <SPI.h>
#include <MySensors.h>
#include "Const.h"
#include "L293D.h"
#include "Dimmer.h"
#include "Bounce.h"
#include "ButtonWithLed.h"

Dimmer dimmer;
ButtonWithLed upButton;
ButtonWithLed downButton;
L293D motor;

bool finishedStartup;

void setup() {
	finishedStartup = false;

	dimmer.Init(LIGHT_PIN, CHILD_ID_LIGHT, BUTTON_PIN, LIGHT_BUTTON_PIN);
	upButton.Init(CHILD_ID_UP, BUTTON_PIN_UP, LIGHT_BUTTON_PIN_UP);
	downButton.Init(CHILD_ID_DOWN, BUTTON_PIN_DOWN, LIGHT_BUTTON_PIN_DOWN);
	motor.Init(CHILD_ID_UP, CHILD_ID_DOWN, BUTTON_MOTOR_UP, BUTTON_MOTOR_DOWN);
}

void presentation()
{
	// Register chils at the gateway
	present(CHILD_ID_LIGHT, S_DIMMER);
	present(CHILD_ID_UP, S_LIGHT);
	present(CHILD_ID_DOWN, S_LIGHT);
  present(CHILD_ID_WAKEUPLIGHT, S_LIGHT);

	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("Bedroom rollerblinds", "2.1");
}

void loop() {
	if (!finishedStartup)
	{
		finishedStartup = true;

		dimmer.SetStatus(true);
		upButton.SetStatus(false);
		downButton.SetStatus(false);
		motor.ImmediateStop();
	}

	motor.Update();
	dimmer.Update();

	// Returns true when changed
	if (upButton.Update()) {
		if (upButton.Status()) {
			// Turn off the other direction
			downButton.SetStatus(false);
			motor.SetDirection(1);
		}
		else {
			motor.ImmediateStop();
		}
	}
	if (downButton.Update()) {
		if (downButton.Status()) {
			// Turn off the other direction
			upButton.SetStatus(false);
			motor.SetDirection(0);
		}
		else {
			motor.ImmediateStop();
		}
	}
}

void receive(const MyMessage &message) {
	Serial.println(message.data);
	Serial.println(message.type);
	Serial.println(message.sensor);

	// We only expect one type of message from controller. But we better check anyway.
	if (message.type != V_LIGHT && message.type != V_DIMMER) {
		return;
	}
	
	//fan.ProcessMessage(message);
	switch (message.sensor)
	{
	case CHILD_ID_UP:
		if (atoi(message.data)) {
			motor.SetDirection(true);
			upButton.SetStatus(true);
			downButton.SetStatus(false);
		}
		else {
			motor.ImmediateStop();
			upButton.SetStatus(false);
			downButton.SetStatus(false);
		}
		break;
	case CHILD_ID_DOWN:
		if (atoi(message.data)) {
			motor.SetDirection(false);
			upButton.SetStatus(false);
			downButton.SetStatus(true);
		}
		else {
			motor.ImmediateStop();
			upButton.SetStatus(false);
			downButton.SetStatus(false);
		}
		break;
	case CHILD_ID_LIGHT:
    if (atoi(message.data)) {
      dimmer.SetStatus(true);
    }
    else {
      dimmer.SetStatus(false);
    }
    break;
  case CHILD_ID_WAKEUPLIGHT:
    if (atoi(message.data)) {
      dimmer.StartWakeupLight();
    }
    break;
  }
}
