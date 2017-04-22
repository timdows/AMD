
#define MY_DEBUG
#define MY_RADIO_NRF24

#define MY_NODE_ID 4

#define MY_PARENT_NODE_ID 0
#define MY_PARENT_NODE_IS_STATIC

#include "FanController.h"
#include <DHT.h>
#include "DHTClass.h"
#include <SPI.h>
#include <MySensors.h>
#include "Const.h"
#include "Bounce.h"
#include "ACS712.h"

Bounce button;

DHTClass dht1;
//DHTClass dht2;

//ACS712 acs712(CHILD_ID_HAL, HAL_PIN);

FanController fan1A;
// FanController fan1B;
// FanController fan1C;
// FanController fan1D;

// FanController fan2A;
// FanController fan2B;
// FanController fan2C;
// FanController fan2D;

bool finishedStartup;

void setup() {
	finishedStartup = false;

	// button.Init(BUTTON_PIN);
	dht1.Init(CHILD_ID_TEMP_1, CHILD_ID_HUM_1, DHT_PIN_1);
	//dht2.Init(CHILD_ID_TEMP, CHILD_ID_HUM, DHT_PIN);

	fan1A.Init(CHILD_ID_FAN_1A, FAN_PIN_1A);
	// fan1B.Init(CHILD_ID_FAN_1B, FAN_PIN_1B);
	// fan1C.Init(CHILD_ID_FAN_1C, FAN_PIN_1C);
	// fan1D.Init(CHILD_ID_FAN_1D, FAN_PIN_1D);

	// fan2A.Init(CHILD_ID_FAN_2A, FAN_PIN_2A);
	// fan2B.Init(CHILD_ID_FAN_2B, FAN_PIN_2B);
	// fan2C.Init(CHILD_ID_FAN_2C, FAN_PIN_2C);
	// fan2D.Init(CHILD_ID_FAN_2D, FAN_PIN_2D);
}

void presentation()
{
	// Register chils at the gateway
	// present(CHILD_ID_HAL, S_POWER);

	present(CHILD_ID_HUM_1, S_HUM);
	present(CHILD_ID_TEMP_1, S_TEMP);

	//present(CHILD_ID_HUM_2, S_HUM);
	//present(CHILD_ID_TEMP_2, S_TEMP);

	present(CHILD_ID_FAN_1A, S_DIMMER);
	// present(CHILD_ID_FAN_1A, S_LIGHT);
	// present(CHILD_ID_FAN_1B, S_LIGHT);
	// present(CHILD_ID_FAN_1C, S_LIGHT);
	// present(CHILD_ID_FAN_1D, S_LIGHT);

	// present(CHILD_ID_FAN_2A, S_LIGHT);
	// present(CHILD_ID_FAN_2B, S_LIGHT);
	// present(CHILD_ID_FAN_2C, S_LIGHT);
	// present(CHILD_ID_FAN_2D, S_LIGHT);


	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("HeaterFan Studie", "2.0");
}

void loop() {
	if (!finishedStartup)
	{
		finishedStartup = true;
		fan1A.SetOn();
		// fan1B.SetOff();
		// fan1C.SetOff();
		// fan1D.SetOff();

		// fan2A.SetOff();
		// fan2B.SetOff();
		// fan2C.SetOff();
		// fan2D.SetOff();
	}

	dht1.Update();
	// acs712.CheckPowerUsage();

	// returns true if button has changed
	// if (button.Update()) {
	// 	Serial.println("Button changed");
	// 	if (button.Status()) {
	// 		Serial.println("Button ON");
	// 		fan1A.SetOn();
	// 		fan1B.SetOn();
	// 		fan1C.SetOn();
	// 		fan1D.SetOn();

	// 		fan2A.SetOn();
	// 		fan2B.SetOn();
	// 		fan2C.SetOn();
	// 		fan2D.SetOn();
	// 	}
	// 	else {
	// 		Serial.println("Button OFF");
	// 		fan1A.SetOff();
	// 		fan1B.SetOff();
	// 		fan1C.SetOff();
	// 		fan1D.SetOff();

	// 		fan2A.SetOff();
	// 		fan2B.SetOff();
	// 		fan2C.SetOff();
	// 		fan2D.SetOff();
	// 	}
	// }
}

void receive(const MyMessage &message) {
	// We only expect one type of message from controller. But we better check anyway.
	if (message.type != V_LIGHT && message.type != V_DIMMER) {
		return;
	}
	
	//fan.ProcessMessage(message);
	switch (message.sensor)
	{
	case CHILD_ID_FAN_1A:
		fan1A.ProcessMessage(message);
		break;
	// case CHILD_ID_FAN_1B:
	// 	fan1B.ProcessMessage(message);
	// 	break;
	// case CHILD_ID_FAN_1C:
	// 	fan1C.ProcessMessage(message);
	// 	break;
	// case CHILD_ID_FAN_1D:
	// 	fan1D.ProcessMessage(message);
	// 	break;
	}
}

