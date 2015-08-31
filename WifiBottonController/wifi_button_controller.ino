#include <Adafruit_NeoPixel.h>
#include "L293D.h"
#include "Fader.h"
#include "SoftButton.h"

#define LED_PIN 6

L293D MotorRed(13, 11);
L293D MotorGreen(12, 10);
L293D MotorBlue(8, 9); 

Fader FaderRed("Red", 0, &MotorRed);
Fader FaderGreen("Green", 1, &MotorGreen);
Fader FaderBlue("Blue", 2, &MotorBlue);

Adafruit_NeoPixel ledstrip = Adafruit_NeoPixel(4, LED_PIN, NEO_GRB + NEO_KHZ800);
SoftButton Button1(2, 1, &ledstrip, 256, 253, 255); //Mirror
SoftButton Button2(3, 2, &ledstrip, 244, 238, 236); //Aanrecht links
SoftButton Button3(4, 3, &ledstrip, 232, 233, 234); //Aanrecht rechts
SoftButton Button4(5, 0, &ledstrip, 217, 220, 219); //Living colors

SoftButton *selectedButton;
bool newButtonSelected;

unsigned long lastButtonChange;

void setup() {
  Serial.begin(9600);
  
  pinMode(Button1.GetPin(), INPUT);
  pinMode(Button2.GetPin(), INPUT);
  pinMode(Button3.GetPin(), INPUT);
  pinMode(Button4.GetPin(), INPUT);

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
    if(FaderRed.ValueChanged() || FaderGreen.ValueChanged() || FaderBlue.ValueChanged()){
      selectedButton->SetColor(FaderRed.GetCurrentValue(), FaderGreen.GetCurrentValue(), FaderBlue.GetCurrentValue());
    }
    
    if(digitalRead(Button1.GetPin()) == 1){
      selectedButton = &Button1;
      selectedButton->FlashButton();
      newButtonSelected = true;
    }
    if(digitalRead(Button2.GetPin()) == 1){
      selectedButton = &Button2;
      selectedButton->FlashButton();
      newButtonSelected = true;
    }
    if(digitalRead(Button3.GetPin()) == 1){
      selectedButton = &Button3;
      selectedButton->FlashButton();
      newButtonSelected = true;
    }
    if(digitalRead(Button4.GetPin()) == 1){
      selectedButton = &Button4;
      selectedButton->FlashButton();
      newButtonSelected = true;
    }
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


