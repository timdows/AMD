/*
 * Relay class
 */
#include "Relay.h"

Relay::Relay(int pin, int childId){
  this->pin = pin;
  this->childId = childId;

  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, 0);
}

void Relay::SetOn(){
  digitalWrite(this->pin, 0); //Relay on = 0
  send(MyMessage(this->childId, V_LIGHT).set(1));
}

void Relay::SetOff(){
  digitalWrite(this->pin, 1);
  send(MyMessage(this->childId, V_LIGHT).set(0));
}

