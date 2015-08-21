/*
 * ACS712 class
 */
#include "ACS712.h"

//Constructor
ACS712::ACS712(MySensor *mySensorGw){
  this->mySensorGw = mySensorGw;
}

void ACS712::CheckPowerUsage(){
  unsigned long currentMillis = millis();
  
  if(currentMillis - this->previousMillis >= this->intervalDelay || this->forceUpdate){
    this->previousMillis = currentMillis;  // Remember the time
    this->forceUpdate = false;
    
    int rawValue = analogRead(this->analogIn);
    Serial.print("Raw Value = " ); // shows pre-scaled value 
    Serial.print(rawValue); 
    
    //double voltageA = (rawValue / 1023.0);
    //int voltage = voltageA * 5000;
    float voltage = (rawValue * (5.0 / 1023.0)) * 1000;
    Serial.print("\tVoltage "); 
    Serial.print(voltage);
    
    int calcValue = abs(this->ACSoffset - voltage);
    Serial.print("\tcalcValue "); // shows pre-scaled value 
    Serial.print(calcValue); // the '3' after voltage allows you to display 3 digits after decimal point
    
    double amps = calcValue / this->mVperAmp;
    Serial.print("\tamps "); // shows pre-scaled value 
    Serial.print(amps, 3); // the '3' after voltage allows you to display 3 digits after decimal point
    
    int watt = amps * 5;
    
    //double voltage = (rawValue / 1023.0) * 5000; // Gets you mV
    //double amps = ((this->ACSoffset - voltage) / this->mVperAmp);
    //uint8_t watt = amps * 5;
//    uint8_t watt = ((((rawValue/1023)*5000)-2500)/this->mVperAmp)*5.0;
    
    
    /*
    Serial.print("\t mV ("); // shows the voltage measured 
    Serial.print(rawValue);
    Serial.print(" / 1023.0) * 5000 = ");
    Serial.print(voltage,3); // the '3' after voltage allows you to display 3 digits after decimal point
    
    Serial.print("\t Amps (2500 - "); // shows the voltage measured 
    Serial.print(voltage,3);
    Serial.print(") / 185 = ");
    Serial.print(amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
    
    Serial.print("\t Watts = "); // shows the voltage measured 
    Serial.println(watt);*/
    
    //3 is the #define CHILD_ALL
    this->mySensorGw->send(MyMessage(3, V_WATT).set(watt));
  }
}

void ACS712::ForceUpdate(){
  this->forceUpdate = true;
}
