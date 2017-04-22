#include "Bounce2.h"
#define MY_DEBUG
#define MY_RADIO_NRF24

#include <SPI.h>
#include <MySensors.h>
#include "Const.h"
#include "LedstripColor.h"
#include "Bounce2.h"

LedstripColor ledAll;
LedstripColor ledRed;
LedstripColor ledGreen;
LedstripColor ledBlue;

Bounce buttonLeft;
Bounce buttonRight;

bool finishedStartup;

void setup() {
	finishedStartup = false;

	ledAll.Init(0, CHILD_ID_ALL, 25);
	ledRed.Init(PIN_RED, CHILD_ID_RED, 25);
	ledGreen.Init(PIN_GREEN, CHILD_ID_GREEN, 25);
	ledBlue.Init(PIN_BLUE, CHILD_ID_BLUE, 25);

	buttonLeft.attach(PIN_LEFT, INPUT_PULLUP);
	buttonRight.attach(PIN_RIGHT, INPUT_PULLUP);
}

void presentation()
{
	// Register chils at the gateway
	present(CHILD_ID_ALL, S_DIMMER);
	present(CHILD_ID_RED, S_DIMMER);
	present(CHILD_ID_GREEN, S_DIMMER);
	present(CHILD_ID_BLUE, S_DIMMER);

	present(CHILD_ID_BUTTON_LEFT, S_DOOR);
	present(CHILD_ID_BUTTON_RIGHT, S_DOOR);

	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("Bedroom LedStrip", "2.0");
}

void loop() {
	if (!finishedStartup)
	{
		finishedStartup = true;

		ledRed.SetDesiredLedLevel(100);
		ledGreen.SetDesiredLedLevel(100);
		ledBlue.SetDesiredLedLevel(100);
	}

	ledRed.Update();
	ledGreen.Update();
	ledBlue.Update();

	if (buttonLeft.update()) {
		send(MyMessage(CHILD_ID_BUTTON_LEFT, V_TRIPPED).set(buttonLeft.read() ? 1 : 0));
	}

	if (buttonRight.update()) {
		send(MyMessage(CHILD_ID_BUTTON_RIGHT, V_TRIPPED).set(buttonRight.read() ? 1 : 0));
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

	//  Retrieve the power or dim level from the incoming request message
	int requestedLevel = atoi(message.data);

	// Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
	requestedLevel *= (message.type == V_LIGHT ? 100 : 1);

	// Clip incoming level to valid range of 0 to 100
	requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
	requestedLevel = requestedLevel < 0 ? 0 : requestedLevel;
	
	switch (message.sensor)
	{
	case CHILD_ID_ALL:
		ChangeALL(requestedLevel);
		break;
	case CHILD_ID_RED:
		ledRed.SetDesiredLedLevel(requestedLevel);
		break;
	case CHILD_ID_GREEN:
		ledGreen.SetDesiredLedLevel(requestedLevel);
		break;
	case CHILD_ID_BLUE:
		ledBlue.SetDesiredLedLevel(requestedLevel);
		break;
	}
}

void ChangeALL(int requestedLevel){
	//Get the highest value to work with
	int highest = ledBlue.GetCurrentLedLevel();
	if (ledRed.GetCurrentLedLevel() > highest) {
		highest = ledRed.GetCurrentLedLevel();
	}
	if (ledGreen.GetCurrentLedLevel() > highest) {
		highest = ledGreen.GetCurrentLedLevel();
	}

	//Calculate the percentage to change all the leds
	double percentToChange = (double)requestedLevel / (double)highest;

	//If no led is on, try to restore the last led value
	if (highest == 0) {
		ledBlue.RestoreLastLedLevel();
		ledRed.RestoreLastLedLevel();
		ledGreen.RestoreLastLedLevel();
	}
	else if (percentToChange > 0) {
		ledBlue.ChangeLevelInPercent(percentToChange);
		ledRed.ChangeLevelInPercent(percentToChange);
		ledGreen.ChangeLevelInPercent(percentToChange);
	}
	else {
		ledBlue.SetDesiredLedLevel(requestedLevel);
		ledRed.SetDesiredLedLevel(requestedLevel);
		ledGreen.SetDesiredLedLevel(requestedLevel);
	}

	Serial.print("requestedLevel ");
	Serial.print(requestedLevel);
	Serial.print(" highest ");
	Serial.print(highest);
	Serial.print(" percent change is ");
	printDouble(percentToChange, 4);
	Serial.println();

	ledAll.SetDesiredLedLevel(requestedLevel);
	ledAll.ForceGwUpdate();
}

void printDouble(double val, byte precision) {
	Serial.print(int(val));                                     // Print int part
	if (precision > 0) {                                         // Print decimal part
		Serial.print(".");
		unsigned long frac, mult = 1;
		byte padding = precision - 1;
		while (precision--) mult *= 10;
		if (val >= 0) frac = (val - int(val)) * mult; else frac = (int(val) - val) * mult;
		unsigned long frac1 = frac;
		while (frac1 /= 10) padding--;
		while (padding--) Serial.print("0");
		Serial.print(frac, DEC);
	}
}