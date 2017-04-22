
#define MY_DEBUG
#define MY_RADIO_NRF24
//#define MY_REPEATER_FEATURE

//#define HEARTBEAT_TIME 60000UL  // 1 minute

/*
	Pas nodige hardware aan in ino
	Pas node ID aan
	Pas aantal led's aan op de strip (WS287xx.h)

*/

#define MY_NODE_ID 19

#include "ACS712.h"
#include "Relay.h"
#include <SPI.h>
#include <MySensors.h>
#include "WS28xx.h"
#include "Adafruit_NeoPixel.h"

#define NUMBER_OF_SCENES 4

#define CHILD_RED 0
#define CHILD_GREEN 1
#define CHILD_BLUE 2
#define CHILD_ALL 3
//Mind the scenes that get automated assigned child numbers
//#define CHILD_RELAY1 21
//#define CHILD_RELAY2 22

//MySensor gw;
WS28xx ledStrip(CHILD_RED, CHILD_GREEN, CHILD_BLUE, CHILD_ALL);
ACS712 acs712(CHILD_ALL, A3);
//Relay relay1(3, CHILD_RELAY1);
//Relay relay2(4, CHILD_RELAY2);

bool finishedStartup;
//unsigned long lastHeartbeatTime;

void setup() {
	finishedStartup = false;
	//lastHeartbeatTime = millis();
}

void presentation()
{
	// Register the LED Dimmable Light with the gateway
	present(CHILD_RED, S_DIMMER);
	present(CHILD_GREEN, S_DIMMER);
	present(CHILD_BLUE, S_DIMMER);
	present(CHILD_ALL, S_DIMMER);

	//present(CHILD_RELAY1, S_LIGHT);
	//present(CHILD_RELAY2, S_LIGHT);

	// Register and get what scene is on
	for (int scene = 1; scene <= NUMBER_OF_SCENES; scene++) {
		// Register all scenes to gw (they will be created as child devices)
		present(scene + 4, S_LIGHT);
	}

	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("WS28xx Entree", "2");
}

void loop() {
	if (!finishedStartup)
	{
		finishedStartup = true;
		ledStrip.SetAllOff();
	}

	//if ((millis() - lastHeartbeatTime) > HEARTBEAT_TIME) {
	//	sendHeartbeat();
	//	lastHeartbeatTime = millis();
	//}

	ledStrip.Update();
	acs712.CheckPowerUsage();
}

void receive(const MyMessage &message) {
	/*Serial.println(message.data);
	Serial.println(message.type);
	Serial.println(message.sensor);*/

	// We only expect one type of message from controller. But we better check anyway.
	if (message.type == V_LIGHT || message.type == V_DIMMER) {
		//Update RGB values
		if (message.sensor < NUMBER_OF_SCENES) {
			ledStrip.SetRGBValue(message);
		}
		
		// else if (message.sensor == CHILD_RELAY1) {
		// 	if (atoi(message.data) == 0) {
		// 		relay1.SetOff();
		// 	}
		// 	else {
		// 		relay1.SetOn();
		// 	}
		// }
		// else if (message.sensor == CHILD_RELAY2) {
		// 	if (atoi(message.data) == 0) {
		// 		relay2.SetOff();
		// 	}
		// 	else {
		// 		relay2.SetOn();
		// 	}
		// }

		//Update scene values
		else {
			// Write some debug info
			Serial.print("Incoming change for sensor:");
			Serial.print(message.sensor);
			Serial.print(", New status: ");
			Serial.println(message.getBool());

			//Tell the ledStrip object to process the scene
			ledStrip.ProcessScene(message.sensor - NUMBER_OF_SCENES, message.getBool());
		}
	}
}

