#include "SoftButton.h"

//Constructor
SoftButton::SoftButton(int row, int column, int ledstripNumber, Adafruit_NeoPixel *ledstrip, int veraIdRed, int veraIdGreen, int veraIdBlue){
  this->row = row;
  this->column = column;
  this->ledstripNumber = ledstripNumber;
  this->ledstrip = ledstrip;

  this->veraIdRed = veraIdRed;
  this->veraIdGreen = veraIdGreen;
  this->veraIdBlue = veraIdBlue;

  this->changeVeraRed = false;
  this->changeVeraGreen = false;
  this->changeVeraBlue = false;
  
  this->shouldFlash = false;
  this->flashNumber = 0;
  this->flashLedStatus = false;
}

int SoftButton::GetRow(){
  return this->row;
}

void SoftButton::SetColor(int red, int green, int blue){
  if(this->currentRed != red){
    this->currentRed = red;
    this->changeVeraRed = true;
  }

  if(this->currentGreen != green){
    this->currentGreen = green;
    this->changeVeraGreen = true;
  }

  if(this->currentBlue != blue){
    this->currentBlue = blue;
    this->changeVeraBlue = true;
  }
  
  this->currentGreen = green;
  this->currentBlue = currentBlue;

  this->SetColorToStrip();
}

void SoftButton::SendCommandToVera(int id, int level){
  Serial.print("setLed(");
  Serial.print(id);
  Serial.print(", ");
  Serial.print(level);
  Serial.println(")");
}

void SoftButton::SetColorToStrip(){
  int red = this->currentRed * 255 / 100;
  int green = this->currentGreen * 255 / 100;
  int blue = this->currentBlue * 255 / 100;
  
  uint32_t color = this->ledstrip->Color(red, green, blue);
  this->ledstrip->setPixelColor(this->ledstripNumber, color);
  this->ledstrip->show();
}

void SoftButton::SetStripWhite(){
  uint32_t color = this->ledstrip->Color(0, 0, 0);
  this->ledstrip->setPixelColor(this->ledstripNumber, color);
  this->ledstrip->show();
}

void SoftButton::FlashButton(){
  this->flashNumber = 0;
  this->shouldFlash = true;
  this->flashLedStatus = false;
}

void SoftButton::Update(){
  unsigned long currentMillis = millis();
  
  if(currentMillis > this->previousMillis + 200 && this->shouldFlash){
    this->previousMillis = currentMillis;  // Remember the time

    this->flashLedStatus = !this->flashLedStatus;
    if(this->flashLedStatus){
      this->SetStripWhite();
    }
    else{
      this->SetColorToStrip();
    }

    if(this->flashNumber++ == 3){
      this->shouldFlash = false;
    }
  }

  if(currentMillis > this->previousMillis + 500 && (this->changeVeraRed || this->changeVeraGreen || this->changeVeraBlue)){
    this->previousMillis = currentMillis;  // Remember the time

    if(this->changeVeraRed){
      this->changeVeraRed = false;
      this->SendCommandToVera(this->veraIdRed, this->currentRed);
    }
    if(this->changeVeraGreen){
      this->changeVeraGreen = false;
      this->SendCommandToVera(this->veraIdGreen, this->currentGreen);
    }
    if(this->changeVeraBlue){
      this->changeVeraBlue = false;
      this->SendCommandToVera(this->veraIdBlue, this->currentBlue);
    }
  }
}

int SoftButton::GetRed(){
  return this->currentRed;
}

int SoftButton::GetGreen(){
  return this->currentGreen;
}

int SoftButton::GetBlue(){
  return this->currentBlue;
}

