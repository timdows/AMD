#include <DHT.h>
#include "DHTClass.h"
#include "Switch.h"
#include "Debounce.h"
#include <MySensor.h>
#include <SPI.h>
#include "Const.h"

MySensor gw;
DHTClass dht;
Debounce waterLevel(&gw, CHILD_ID_WATERLEVEL, PIN_WATERLEVEL);
Switch mist(&gw, CHILD_ID_MIST, PIN_MIST);
Switch anion(&gw, CHILD_ID_ANION, PIN_ANION);
Switch fan(&gw, CHILD_ID_FAN, PIN_FAN);

void setup()
{
	gw.begin(incomingMessage);

	dht.Init(&gw, CHILD_ID_TEMP, CHILD_ID_HUM, PIN_DHT);

	// Send the sketch version information to the gateway and Controller
	gw.sendSketchInfo("Air humidifier", "1.0");

	// Register all sensors and actors to gw (they will be created as child devices)
	gw.present(CHILD_ID_MIST, S_LIGHT);
	gw.present(CHILD_ID_ANION, S_LIGHT);
	gw.present(CHILD_ID_FAN, S_LIGHT);
	gw.present(CHILD_ID_WATERLEVEL, S_DOOR);
	gw.present(CHILD_ID_HUM, S_HUM);
	gw.present(CHILD_ID_TEMP, S_TEMP);

	//setAllOff();
	setAllOn();
}

void loop()
{
	gw.process();
	dht.Update();
	waterLevel.Update();

	if (waterLevel.HasChanged())
	{
		if (waterLevel.GetValue())
		{
			Serial.println("Water level low");
			setAllOff();
		}
		else {
			Serial.println("Water level high");
		}
	}
}

void incomingMessage(const MyMessage &message) {
	Serial.println(message.type);
	Serial.println(message.sensor);
	Serial.println(message.data);

	if (waterLevel.GetValue())
	{
		Serial.println("Water level low");
		return;
	}

	if (message.sensor == CHILD_ID_MIST && message.type == V_LIGHT)
	{
		if (message.getBool())
		{
			mist.SetOn(false);
			if (!fan.GetState())
			{
				fan.SetOn(true);
			}
		}
		else {
			mist.SetOff(false);
		}
	}

	if (message.sensor == CHILD_ID_ANION && message.type == V_LIGHT)
	{
		if (message.getBool())
		{
			anion.SetOn(false);
			if (!fan.GetState())
			{
				fan.SetOn(true);
			}
		}
		else {
			anion.SetOff(false);
		}
	}

	if (message.sensor == CHILD_ID_FAN && message.type == V_LIGHT)
	{
		if (message.getBool())
		{
			fan.SetOn(false);
		}
		else {
			fan.SetOff(false);
			if (mist.GetState())
			{
				mist.SetOff(true);
			}
			if (anion.GetState())
			{
				anion.SetOff(true);
			}
		}
	}
}

void setAllOff()
{
	mist.SetOff(true);
	anion.SetOff(true);
	fan.SetOff(true);
}

void setAllOn()
{
	waterLevel.Update();
	if (waterLevel.GetValue())
	{
		Serial.println("Water level low");
		setAllOff();
	}
	else 
	{
		fan.SetOn(true);
		mist.SetOn(true);
		anion.SetOn(true);
	}
}