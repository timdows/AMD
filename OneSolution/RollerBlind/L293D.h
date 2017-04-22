#ifndef H_L293D
#define H_L293D

#include "Arduino.h"
#include "MySensorsCore.h"

class L293D {
private:
	int pin_motor_1;
	int pin_motor_2;
	unsigned long previousMillis;

	MyMessage motorUpMsg;
	MyMessage motorDownMsg;

	boolean running;
	int runningSpeed;
	//unsigned long runningMilliseconds;

	//0 = down
	//1 = up
	int runningDirection;

	void Run();
	void Stop();

public:
	//L293D(int motorUp, int motorDown, int pin1, int pin2);
	void Init(int childIdUp, int childIdDown, int pin1, int pin2);
	void Update();
	//void SetRunningMilliseconds(unsigned long milliseconds);
	void SetDirection(int direction);
	void ImmediateStop();

};

#endif

