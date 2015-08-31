#ifndef H_FADER
#define H_FADER

#include "Arduino.h"
#include "L293D.h"

class Fader{
private:
  int pin;
  L293D *motor;
  String name;

  int currentValue;
  int lastValue;
  bool valueChanged;

  int desiredValue;
  bool moveMotor;
  int moveDirection;
  unsigned long motorStartedMilliseconds;

  int GetAnalogValue(int sensorValue);
public:
  Fader(String name, int pin, L293D *motor);
  void Update();
  bool ValueChanged();
  int GetCurrentValue();
  int GetLastValue();
  void SetDesiredValue(int desiredValue);
};

#endif
