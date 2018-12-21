// 
// 
// 

#include "Dimmer.h"

void Dimmer::Init(int ledPin, int childId, int buttonPin, int buttonLedPin)
{
	this->ledPin = ledPin;
	this->childId = childId;
	this->buttonPin = buttonPin;
	this->buttonLedPin = buttonLedPin;

	this->lightButton.Init(this->buttonPin);
	pinMode(this->ledPin, OUTPUT);
	pinMode(this->buttonLedPin, OUTPUT);

	send(MyMessage(this->childId, V_LIGHT).set(0));
}

void Dimmer::Update()
{
	//Serial.println(this->shouldChange);

	// returns true if button has changed
	if (this->lightButton.Update() && !this->shouldChange) {
		this->shouldChange = true; 
		
		this->goOnOff = !this->goOnOff;

   // If a button is pressed or message received, set all to false
    this->wakeupLightStarted1 = false;
    this->wakeupLightStarted2 = false;
    this->wakeupLightStarted3 = false;
    this->wakeupLightStarted4 = false;
    this->wakeupLightStarted5 = false;
    this->wakeupLightStarted6 = false;
    this->wakeupLightStarted7 = false;
    this->wakeupLightStarted8 = false;
    this->wakeupLightStarted9 = false;
		/*Serial.print("New state");
		Serial.println(this->goOnOff);*/
	}

	unsigned long currentMillis = millis();

	if (currentMillis - this->previousMillis >= this->dimmerDelay && this->shouldChange) {
		this->previousMillis = currentMillis;  // Remember the time
		
		if (this->goOnOff) {
			if (this->currentBrightness++ >= 100) {
				this->currentBrightness = 100;
				this->shouldChange = false;
				return;
			}
		}
		else {
			if (this->currentBrightness-- <= 0) {
				this->currentBrightness = 0;
				this->shouldChange = false;
				return;
			}
		}

		/*Serial.print("currentBrightness ");
		Serial.println(this->currentBrightness);*/

		int value = (int)(this->currentBrightness / 100. * 255);

		analogWrite(this->ledPin, value);
		analogWrite(this->buttonLedPin, value);
	}

 // Part for the wakeup light
 if (this->wakeupLightStarted9 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 9) {
  this->wakeupLightStarted9 = false;
  analogWrite(this->ledPin, 50);
  analogWrite(this->buttonLedPin, 50);
 }
 if (this->wakeupLightStarted8 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 8) {
  this->wakeupLightStarted8 = false;
  this->wakeupLightStarted9 = true;
  analogWrite(this->ledPin, 40);
  analogWrite(this->buttonLedPin, 40);
 }
 if (this->wakeupLightStarted7 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 7) {
  this->wakeupLightStarted7 = false;
  this->wakeupLightStarted8 = true;
  analogWrite(this->ledPin, 30);
  analogWrite(this->buttonLedPin, 30);
 }
 if (this->wakeupLightStarted6 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 6) {
  this->wakeupLightStarted6 = false;
  this->wakeupLightStarted7 = true;
  analogWrite(this->ledPin, 20);
  analogWrite(this->buttonLedPin, 20);
 }
 if (this->wakeupLightStarted5 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 5) {
  this->wakeupLightStarted5 = false;
  this->wakeupLightStarted6 = true;
  analogWrite(this->ledPin, 15);
  analogWrite(this->buttonLedPin, 15);
 }
 if (this->wakeupLightStarted4 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 4) {
  this->wakeupLightStarted4 = false;
  this->wakeupLightStarted5 = true;
  analogWrite(this->ledPin, 10);
  analogWrite(this->buttonLedPin, 10);
 }
 if (this->wakeupLightStarted3 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 3) {
  this->wakeupLightStarted3 = false;
  this->wakeupLightStarted4 = true;
  analogWrite(this->ledPin, 5);
  analogWrite(this->buttonLedPin, 5);
 }
 if (this->wakeupLightStarted2 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 2) {
  this->wakeupLightStarted2 = false;
  this->wakeupLightStarted3 = true;
  analogWrite(this->ledPin, 3);
  analogWrite(this->buttonLedPin, 3);
 }
 if (this->wakeupLightStarted1 && currentMillis - this->wakeupLightStartMillis >= this->wakeupLightDelay * 1) {
  this->wakeupLightStarted1 = false;
  this->wakeupLightStarted2 = true;
  analogWrite(this->ledPin, 2);
  analogWrite(this->buttonLedPin, 2);
 }
}

void Dimmer::SetStatus(bool value)
{
	this->shouldChange = true;
	this->goOnOff = value;

  // If a button is pressed or message received, set all to false
  this->wakeupLightStarted1 = false;
  this->wakeupLightStarted2 = false;
  this->wakeupLightStarted3 = false;
  this->wakeupLightStarted4 = false;
  this->wakeupLightStarted5 = false;
  this->wakeupLightStarted6 = false;
  this->wakeupLightStarted7 = false;
  this->wakeupLightStarted8 = false;
  this->wakeupLightStarted9 = false;
}

void Dimmer::StartWakeupLight()
{
  if (!this->wakeupLightStarted1)
  {
    this->goOnOff = true;
    
    this->wakeupLightStartMillis = millis();

    this->wakeupLightStarted1 = true;
    this->wakeupLightStarted2 = false;
    this->wakeupLightStarted3 = false;
    this->wakeupLightStarted4 = false;
    this->wakeupLightStarted5 = false;
    this->wakeupLightStarted6 = false;
    this->wakeupLightStarted7 = false;
    this->wakeupLightStarted8 = false;
    this->wakeupLightStarted9 = false;
    
    analogWrite(this->ledPin, 1);
    analogWrite(this->buttonLedPin, 1);
  }
}
