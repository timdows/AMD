#ifndef H_DEBOUNCE
#define H_DEBOUNCE

#include "Arduino.h"
#include <MySensor.h> 
#include "Bounce2.h"

class Debounce {
private:
	int pin_switch;
	int currentValue;
	boolean hasChanged;

	MySensor *mySensorGw;
	MyMessage debounceMsg;

	Bounce debouncer;// = Bounce(); 


public:
	Debounce(MySensor *mySensorGw, int child, int pin);
	void Update();
	boolean HasChanged();
	int GetValue();

};

#endif
