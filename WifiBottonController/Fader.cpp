#include "Fader.h"

//Constructor
Fader::Fader(String name, int pin, L293D *motor){
  this->name = name;
  this->pin = pin;
  this->motor = motor;
  this->valueChanged = true;
  this->desiredValue = 0;
  this->moveMotor = false;
  this->moveDirection = 0;
}

void Fader::Update(){
  this->motor->Update();
  
  int newReading = this->GetAnalogValue(analogRead(this->pin));
  
  if(newReading != this->lastValue){
    this->lastValue = this->currentValue;
    this->currentValue = newReading;

    if(newReading != this->lastValue + 5 && newReading != this->lastValue - 5){
      this->valueChanged = true;
    }
  }
  
  if(this->moveMotor){
    /*Serial.print(this->name);
    Serial.print(" desired ");
    Serial.print(this->desiredValue);
    Serial.print(" current ");
    Serial.println(this->currentValue);*/

    if(this->desiredValue == currentValue){
      this->moveMotor = false;
      this->motor->ImmediateStop();
    }
    //Stop after running for 2 seconds
    else if(millis() > this->motorStartedMilliseconds + 2000){
      this->moveMotor = false;
      this->motor->ImmediateStop();
    }
    else{
      //It overmoved?
      if(this->desiredValue > currentValue && this->moveDirection != 1){
        //this->moveDirection = 1;
        this->moveMotor = false;
        this->motor->ImmediateStop();
      }
      else if(this->desiredValue < currentValue && this->moveDirection != 0){
        //this->moveDirection = 0;
        this->moveMotor = false;
        this->motor->ImmediateStop();
      }
      else{
        this->motor->SetDirection(this->moveDirection);
        this->motor->SetRunningMilliseconds(100);
      }
    }  
  }
}

bool Fader::ValueChanged(){
  //Dont tell that the value changed when the motor is doing its thing
  if(this->moveMotor){
    return false;
  }
  return this->valueChanged;
}

int Fader::GetCurrentValue(){
  this->valueChanged = false;
  return this->currentValue;
}

int Fader::GetLastValue(){
  return this->lastValue;
}

void Fader::SetDesiredValue(int desiredValue){
  /*Serial.print("Setting desired value for ");
  Serial.print(this->name);
  Serial.print(" to value ");
  Serial.println(desiredValue);*/
  this->desiredValue = desiredValue;
  this->moveMotor = true;
  this->motorStartedMilliseconds = millis();

  //Check what direction the motor should move
  if(this->desiredValue > currentValue){
    this->moveDirection = 1;
  }
  else{
    this->moveDirection = 0;
  }

  this->motor->SetDirection(this->moveDirection);
}

int Fader::GetAnalogValue(int sensorValue){
  /*Serial.print("PIN ");
  Serial.print(this->pin);
  Serial.print(" ");
  Serial.println(sensorValue);*/
  if(sensorValue > 980){
    return 100;
  }
  if(sensorValue > 950){
    return 95;
  }
  if(sensorValue > 900){
    return 90;
  }
  if(sensorValue > 850){
    return 85;
  }
  if(sensorValue > 800){
    return 80;
  }
  if(sensorValue > 750){
    return 75;
  }
  if(sensorValue > 700){
    return 70;
  }
  if(sensorValue > 650){
    return 65;
  }
  if(sensorValue > 600){
    return 60;
  }
  if(sensorValue > 550){
    return 55;
  }
  if(sensorValue > 500){
    return 50;
  }
  if(sensorValue > 450){
    return 45;
  }
  if(sensorValue > 400){
    return 40;
  }
  if(sensorValue > 350){
    return 35;
  }
  if(sensorValue > 300){
    return 30;
  }
  if(sensorValue > 250){
    return 25;
  }
  if(sensorValue > 200){
    return 20;
  }
  if(sensorValue > 150){
    return 15;
  }
  if(sensorValue > 100){
    return 10;
  }
  return 0;
}
