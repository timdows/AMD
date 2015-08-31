#ifndef H_L293D
#define H_L293D

#include "Arduino.h"

class L293D{
private:
  int pin_motor_1;
  int pin_motor_2;
  unsigned long previousMillis;
  
  boolean running;
  int runningSpeed;
  unsigned long runningMilliseconds;
  
  //0 = down
  //1 = up
  int runningDirection;
  
  void Run();
  void Stop();
  
public:
  L293D(int pin1, int pin2);
  void Update();
  void SetRunningMilliseconds(unsigned long milliseconds);
  void SetDirection(int direction);
  void ImmediateStop();
  
};

#endif
