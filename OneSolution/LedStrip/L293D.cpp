/*
* L293D class
*/
#include "L293D.h"

//Constructor
//L293D::L293D(int motorUp, int motorDown, int pin1, int pin2) {
void L293D::Init(int childIdUp, int childIdDown, int pin1, int pin2)
{
	this->pin_motor_1 = pin1;
	this->pin_motor_2 = pin2;

	pinMode(this->pin_motor_1, OUTPUT);
	pinMode(this->pin_motor_2, OUTPUT);

	this->previousMillis = millis();

	//this->motorUpMsg = MyMessage(motorUp, V_LIGHT);
	//this->motorDownMsg = MyMessage(motorDown, V_LIGHT);

	this->running = false;
	this->runningSpeed = 255;
	//this->runningMilliseconds = 0;
	this->runningDirection = 0;
}

//void L293D::SetRunningMilliseconds(unsigned long milliseconds) {
//	this->runningMilliseconds = milliseconds;
//}

void L293D::SetDirection(int direction) {
	//Stop whatever was running
	if (this->running) {
		this->Stop();
	}
	this->runningDirection = direction;
	this->running = true;
}

void L293D::Update() {
	//unsigned long currentMillis = millis();

	//if (this->runningMilliseconds > 0 && !this->running) {
	if(this->running){
		//Serial.println("going on");
		//this->previousMillis = currentMillis;  // Remember the time

											   //start
		this->Run();
	}

	//if (this->running && (currentMillis - this->previousMillis >= this->runningMilliseconds)) {
	//	//Serial.println("going off");
	//	this->previousMillis = currentMillis;  // Remember the time
	//	this->runningMilliseconds = 0;

	//	//stop
	//	this->Stop();
	//}

	/*Serial.print(currentMillis - this->previousMillis);
	Serial.print(" - ");
	Serial.println(this->runningMilliseconds);*/
}

void L293D::Run() {
	this->running = true;


	//down
	if (this->runningDirection == 0) {
		analogWrite(this->pin_motor_1, this->runningSpeed);
		analogWrite(this->pin_motor_2, 0);
	}
	//up
	else if (this->runningDirection == 1) {
		analogWrite(this->pin_motor_1, 0);
		analogWrite(this->pin_motor_2, this->runningSpeed);
	}
}

void L293D::Stop() {
	this->running = false;

	analogWrite(this->pin_motor_1, 0);
	analogWrite(this->pin_motor_2, 0);

	//down
	if (this->runningDirection == 0) {
		//Serial.println("sending gw message down in class");
		//MyMessage(this->motorDownMsg.set(0));
	}
	//up
	else if (this->runningDirection == 1) {
		//Serial.println("sending gw message up in class");
		//MyMessage(this->motorDownMsg.set(0));
	}
}

void L293D::ImmediateStop() {
	//this->runningMilliseconds = 0;
	this->running = false;
	this->Stop();
}
