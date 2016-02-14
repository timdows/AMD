#include "ACS712.h"
#include "FanController.h"
#include "DHTClass.h"
#include <MySensor.h>
#include <SPI.h>
#include <DHT.h>
#include "Const.h"

ACS712 acs712;
DHTClass dht1;
FanController fan1A;
FanController fan1B;
FanController fan1C;
FanController fan1D;

MySensor gw;

void setup()
{
	// Initialize library and add callback for incoming messages
	gw.begin(incomingMessage, AUTO, false, 3);

	//Startup
	acs712.Init(&gw, CHILD_ID_HAL, HAL_PIN);

	dht1.Init(&gw, CHILD_ID_TEMP_1, CHILD_ID_HUM_1, DHT_PIN_1);
	//dht2.Init(&gw, CHILD_ID_TEMP_2, CHILD_ID_HUM_2, DHT_PIN_2);

	fan1A.Init(&gw, CHILD_ID_FAN_1A, FAN_PIN_1A);
	fan1B.Init(&gw, CHILD_ID_FAN_1B, FAN_PIN_1B);
	fan1C.Init(&gw, CHILD_ID_FAN_1C, FAN_PIN_1C);
	fan1D.Init(&gw, CHILD_ID_FAN_1D, FAN_PIN_1D);

	// Send the sketch version information to the gateway and Controller
	gw.sendSketchInfo("Heater fans", "1.1");

	// Register all sensors to gw (they will be created as child devices)
	gw.present(CHILD_ID_HAL, S_POWER);

	gw.present(CHILD_ID_HUM_1, S_HUM);
	gw.present(CHILD_ID_TEMP_1, S_TEMP);

	gw.present(CHILD_ID_HUM_2, S_HUM);
	gw.present(CHILD_ID_TEMP_2, S_TEMP);
	
	gw.present(CHILD_ID_FAN_1A, S_LIGHT);
	gw.present(CHILD_ID_FAN_1B, S_LIGHT);
	gw.present(CHILD_ID_FAN_1C, S_LIGHT);
	gw.present(CHILD_ID_FAN_1D, S_LIGHT);

	gw.present(CHILD_ID_FAN_2A, S_LIGHT);
	gw.present(CHILD_ID_FAN_2B, S_LIGHT);
	gw.present(CHILD_ID_FAN_2C, S_LIGHT);
	gw.present(CHILD_ID_FAN_2D, S_LIGHT);
}

void loop() {
	gw.process();
	//acs712.ForceUpdate();
	acs712.CheckPowerUsage();
	dht1.Update();
}

void incomingMessage(const MyMessage &message) {

	if (message.type != V_LIGHT)
	{
		return;
	}

	acs712.ForceUpdate();

	switch (message.sensor)
	{
	case CHILD_ID_FAN_1A:
		fan1A.ProcessMessage(message);
		break;
	case CHILD_ID_FAN_1B:
		fan1B.ProcessMessage(message);
		break;
	case CHILD_ID_FAN_1C:
		fan1C.ProcessMessage(message);
		break;
	case CHILD_ID_FAN_1D:
		fan1D.ProcessMessage(message);
		break;
	}
}