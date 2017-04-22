#define MY_DEBUG
#define MY_RADIO_NRF24

#define MY_NODE_ID 9

#define CHILD_ID_DS 0
#define CHILD_ID_HUM 1
#define CHILD_ID_TEMP 2

#define PIN_ONE_WIRE 5 // Pin where dallase sensor is connected 
#define PIN_DHT 3

#include <MySensors.h>
#include <SPI.h>
#include <DHT.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "DHTClass.h"
#include "Dallas.h"

DHTClass dht;
DallasClass dallas;

unsigned long SLEEP_TIME = 300000; // Sleep time between reads (in milliseconds)

void setup()
{
	analogReference(INTERNAL);
}

void presentation()
{
	//Startup
	dht.Init(CHILD_ID_TEMP, CHILD_ID_HUM, PIN_DHT);
	dallas.Init(CHILD_ID_DS, PIN_ONE_WIRE);

	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("Outside Temp", "2.1");

	// Register all sensors to gw (they will be created as child devices)
	present(CHILD_ID_HUM, S_HUM);
	present(CHILD_ID_TEMP, S_TEMP);
	present(CHILD_ID_DS, S_TEMP);
}

// http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long getBatteryVoltage()
{
	// Read 1.1V reference against AVcc
	// set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = _BV(MUX3) | _BV(MUX2);
#else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif  

	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Start conversion
	while (bit_is_set(ADCSRA, ADSC)); // measuring

	uint8_t low = ADCL; // must read ADCL first - it then locks ADCH  
	uint8_t high = ADCH; // unlocks both

	long result = (high << 8) | low;

	result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
	return result; // Vcc in millivolts
}

void loop() {
	dht.ReadSensor();
	dallas.ReadSensor();

	long batteryVoltage = getBatteryVoltage();
	int patteryPercent = batteryVoltage / 50;
	sendBatteryLevel(patteryPercent);

	sleep(SLEEP_TIME);
}

