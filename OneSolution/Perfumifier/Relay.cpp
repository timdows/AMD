/*
 * Relay class
 */
#include "Relay.h"

Relay::Relay(int pin, int childId){
  this->pin = pin;
  this->childId = childId;

  pinMode(this->pin, OUTPUT);
  //digitalWrite(this->pin, 0);
  this->SetOff(true);
}

void Relay::SetOn(bool sendMessage){
  digitalWrite(this->pin, 1); 
  if (sendMessage) {
   send(MyMessage(this->childId, V_LIGHT).set(1)); 
  }
}

void Relay::SetOff(bool sendMessage){
  digitalWrite(this->pin, 0);
  if (sendMessage) {
   send(MyMessage(this->childId, V_LIGHT).set(0)); 
  }
}

