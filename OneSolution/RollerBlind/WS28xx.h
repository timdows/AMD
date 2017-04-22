// WS28xx.h

#ifndef _WS28XX_h
#define _WS28XX_h

#define MY_DEBUG
#define MY_RADIO_NRF24

#define NUMBER_OF_SCENES 4
#define NUM_LEDS 209 //plafond
//#define NUM_LEDS 35 //keukenkastje rechts

#include "arduino.h"
#include "MySensorsCore.h"
#include "Adafruit_NeoPixel.h"
#include "SceneStatus.h"

#define LED_PIN 6

class WS28xx {
private:
	Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);
	//  Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, LED_PIN, NEO_RGB + NEO_KHZ800);
	//boolean sceneStatus[NUMBER_OF_SCENES];// = { 0, 0, 0, 0, 0 };
	SceneStatus sceneStatus;
	
	//Mysensor child ID's
	uint8_t childAll;
	uint8_t childRed;
	uint8_t childGreen;
	uint8_t childBlue;

	//RGB values
	int redRGB = 0;
	int greenRGB = 0;
	int blueRGB = 0;

	//states
	int currentColorWipe = 0;

	int currentKnightRider = 0;
	int delayKnightRider = 5;

	int currentRainbowCycle = 0;
	int delayRainbowCycle = 10;

	int currentTheaterChase = 0;
	int delayTheaterChase = 10;

	int delayDisco = 60;

	boolean currentFullRGB = false;
	boolean allScenesOff = true;

	int intervalDelay = 20;
	unsigned long previousMillis;

	void ColorWipe(uint32_t color);
	void KnightRider();
	void RainbowCycle();
	void Disco();
	void FullRGBColor(boolean state);
	uint32_t Wheel(byte WheelPos);
	void PrintDouble(double val, byte precision);
public:
	WS28xx(uint8_t childAll, uint8_t childRed, uint8_t childGreen, uint8_t childBlue);
	void SetAllOff();
	void Update();
	void ProcessScene(int sensor, bool state);
	void SetRGBValue(const MyMessage &message);
};

#endif

