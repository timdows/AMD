#include <Adafruit_NeoPixel.h>
#include "L293D.h"
#include "Fader.h"
#include "SoftButton.h"

#define LED_PIN 9

#define ROW0 10
#define ROW1 11
#define ROW2 12
#define ROW3 13

#define COLUMN0 18
#define COLUMN1 19
#define COLUMN2 8
#define COLUMN3 17

L293D MotorRed(2, 5);
L293D MotorGreen(3, 4);
L293D MotorBlue(6, 7); 

Fader FaderRed("Red", 2, &MotorRed);
Fader FaderGreen("Green", 1, &MotorGreen);
Fader FaderBlue("Blue", 0, &MotorBlue);

Adafruit_NeoPixel ledstrip = Adafruit_NeoPixel(16, LED_PIN, NEO_GRB + NEO_KHZ800);
SoftButton Button1(ROW0, COLUMN0, 0, &ledstrip, 0, 0, 0);
SoftButton Button2(ROW1, COLUMN0, 1, &ledstrip, 244, 238, 236); //Aanrecht links
SoftButton Button3(ROW2, COLUMN0, 2, &ledstrip, 232, 233, 234); //Aanrecht rechts
SoftButton Button4(ROW3, COLUMN0, 3, &ledstrip, 217, 220, 219); //Living colors

SoftButton Button5(ROW0, COLUMN1, 7, &ledstrip, 150, 151, 152); //Huiskamer plafond   
SoftButton Button6(ROW1, COLUMN1, 6, &ledstrip, 177, 176, 175); //Keuken ledstrip
SoftButton Button7(ROW2, COLUMN1, 5, &ledstrip, 202, 203, 201); //Koelkast led
SoftButton Button8(ROW3, COLUMN1, 4, &ledstrip, 34, 35, 36); //Pianowall

SoftButton Button9(ROW0, COLUMN2, 8, &ledstrip, 267, 262, 261); //TV meubel
SoftButton Button10(ROW1, COLUMN2, 9, &ledstrip, 256, 253, 255); //Mirror
SoftButton Button11(ROW2, COLUMN2, 10, &ledstrip, 0, 0, 0);
SoftButton Button12(ROW3, COLUMN2, 11, &ledstrip, 0, 0, 0);

SoftButton Button13(ROW0, COLUMN3, 15, &ledstrip, 0, 0, 0);
SoftButton Button14(ROW1, COLUMN3, 14, &ledstrip, 0, 0, 0);
SoftButton Button15(ROW2, COLUMN3, 13, &ledstrip, 0, 0, 0);
SoftButton Button16(ROW3, COLUMN3, 12, &ledstrip, 0, 0, 0);

//Entree kerst            135
//Entree schakelaar       62
//Eettafel                24
//Eettafel dim            260
//Zonnelamp               10
//Slaapkamer (groot)      139
//Slaapkamer (dim)        141
//Bosnialamp              168


SoftButton *selectedButton;
bool newButtonSelected;

unsigned long lastButtonChange;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting wifi button controller");

  digitalWrite(COLUMN0, LOW);
  digitalWrite(COLUMN1, LOW);
  //digitalWrite(COLUMN2, LOW);
  //digitalWrite(COLUMN3, LOW);

  digitalWrite(COLUMN0, HIGH);
  digitalWrite(COLUMN1, HIGH);
  analogWrite(COLUMN2, 250);
  analogWrite(COLUMN3, 250);
  
  pinMode(ROW0, INPUT);
  pinMode(ROW1, INPUT);
  pinMode(ROW2, INPUT);
  pinMode(ROW3, INPUT);

  ledstrip.begin();
  ledstrip.show();

  selectedButton = &Button1;
  newButtonSelected = false;
}
 
void loop() {
  FaderRed.Update();
  FaderGreen.Update();
  FaderBlue.Update();
  
  selectedButton->Update();

  if(millis() > lastButtonChange + 800){
    //Serial.println("Reading...");
    if(FaderRed.ValueChanged() || FaderGreen.ValueChanged() || FaderBlue.ValueChanged()){
      selectedButton->SetColor(FaderRed.GetCurrentValue(), FaderGreen.GetCurrentValue(), FaderBlue.GetCurrentValue());
    }

    digitalWrite(COLUMN0, HIGH);
    delay(50);
    
    if(digitalRead(Button1.GetRow()) == 1){
      selectedButton = &Button1;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 1 selected");
    }
    if(digitalRead(Button2.GetRow()) == 1){
      selectedButton = &Button2;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 2 selected");
    }
    if(digitalRead(Button3.GetRow()) == 1){
      selectedButton = &Button3;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 3 selected");
    }
    if(digitalRead(Button4.GetRow()) == 1){
      selectedButton = &Button4;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 4 selected");
    }

    digitalWrite(COLUMN0, LOW);
    digitalWrite(COLUMN1, HIGH);
    delay(50);

    if(digitalRead(Button5.GetRow()) == 1){
      selectedButton = &Button5;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 5 selected");
    }
    if(digitalRead(Button6.GetRow()) == 1){
      selectedButton = &Button6;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 6 selected");
    }
    if(digitalRead(Button7.GetRow()) == 1){
      selectedButton = &Button7;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 7 selected");
    }
    if(digitalRead(Button8.GetRow()) == 1){
      selectedButton = &Button8;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 8 selected");
    }

    digitalWrite(COLUMN1, LOW);
    digitalWrite(COLUMN2, HIGH);
    delay(50);

    if(digitalRead(Button9.GetRow()) == 1){
      selectedButton = &Button9;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 9 selected");
    }
    if(digitalRead(Button10.GetRow()) == 1){
      selectedButton = &Button10;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 10 selected");
    }
    if(digitalRead(Button11.GetRow()) == 1){
      selectedButton = &Button11;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 11 selected");
    }
    if(digitalRead(Button12.GetRow()) == 1){
      selectedButton = &Button12;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 12 selected");
    }

    digitalWrite(COLUMN2, LOW);
    digitalWrite(COLUMN3, HIGH);
    delay(50);

    if(digitalRead(Button13.GetRow()) == 1){
      selectedButton = &Button13;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 13 selected");
    }
    if(digitalRead(Button14.GetRow()) == 1){
      selectedButton = &Button14;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 14 selected");
    }
    if(digitalRead(Button15.GetRow()) == 1){
      selectedButton = &Button15;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 15 selected");
    }
    if(digitalRead(Button16.GetRow()) == 1){
      selectedButton = &Button16;
      selectedButton->FlashButton();
      newButtonSelected = true;
      Serial.println("Button 16 selected");
    }

    digitalWrite(COLUMN3, LOW);
    
  }

  if(newButtonSelected){
    lastButtonChange = millis();
    newButtonSelected = false;
    FaderRed.SetDesiredValue(selectedButton->GetRed());
    FaderGreen.SetDesiredValue(selectedButton->GetGreen());
    FaderBlue.SetDesiredValue(selectedButton->GetBlue());
  }
/*
  Serial.print("FaderRed ");
  Serial.print(FaderRed.GetCurrentValue());
  Serial.print("\tFaderGreen ");
  Serial.print(FaderGreen.GetCurrentValue());
  Serial.print("\tFaderBlue ");
  Serial.println(FaderBlue.GetCurrentValue());
*/
}


