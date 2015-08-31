/*
 * L293D class
 */
#include "L293D.h"

//Constructor
L293D::L293D(int pin1, int pin2){
  this->pin_motor_1 = pin1;
  this->pin_motor_2 = pin2;
  
  pinMode(this->pin_motor_1, OUTPUT);
  pinMode(this->pin_motor_2, OUTPUT);
  
  this->previousMillis = millis();

  this->running = false;
  this->runningSpeed = 200;
  this->runningMilliseconds = 0;
  this->runningDirection = 0;
}

void L293D::SetRunningMilliseconds(unsigned long milliseconds){
  this->runningMilliseconds = milliseconds;
}

void L293D::SetDirection(int direction){
  //Stop whatever was running, maybe not due to hickups
  //if(this->running){
  //  this->Stop();
  //}
  this->runningDirection = direction;
}

void L293D::Update(){
  unsigned long currentMillis = millis();
  
  if(this->runningMilliseconds > 0 && !this->running){
    /*Serial.print("going on  ");
    Serial.println(millis());*/
    this->previousMillis = currentMillis;  // Remember the time
    
    //start
    this->Run();
  }
  
  if(this->running && (currentMillis - this->previousMillis >= this->runningMilliseconds)){
    /*Serial.print("going off ");
    Serial.println(millis());*/
    this->previousMillis = currentMillis;  // Remember the time
    this->runningMilliseconds = 0;
    
    //stop
    this->Stop();
  }
  
  /*Serial.print(currentMillis - this->previousMillis);
  Serial.print(" - ");
  Serial.println(this->runningMilliseconds);*/
}

void L293D::Run(){
  this->running = true;

  //down
  if(this->runningDirection == 0){
    analogWrite(pin_motor_1, this->runningSpeed);
    analogWrite(pin_motor_2, 0);
  }
  //up
  else if(this->runningDirection == 1){
    analogWrite(pin_motor_1, 0);
    analogWrite(pin_motor_2, this->runningSpeed);
  }
}

void L293D::Stop(){
  this->running = false;
  
  analogWrite(pin_motor_1, 0);
  analogWrite(pin_motor_2, 0);
}

void L293D::ImmediateStop(){
  this->runningMilliseconds = 0;
  //this->running = false;
  this->Stop();
}

