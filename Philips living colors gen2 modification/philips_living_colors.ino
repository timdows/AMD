#include <MySensor.h> 
#include <SPI.h>
#include "LedstripColor.h"

//PINS
#define BLUE_PIN 3
#define RED_PIN 5
#define GREEN_PIN 6

//MySensors info
#define SN "Living colors"
#define SV "0.2"
#define CHILD_BLUE 0
#define CHILD_RED 1
#define CHILD_GREEN 2
#define CHILD_ALL 3

MySensor gw;

#define FADE_DELAY 25

//State machine
LedstripColor ledBlue(gw, BLUE_PIN, CHILD_BLUE, FADE_DELAY);
LedstripColor ledRed(gw, RED_PIN, CHILD_RED, FADE_DELAY);
LedstripColor ledGreen(gw, GREEN_PIN, CHILD_GREEN, FADE_DELAY);
LedstripColor ledAll(gw, 0, CHILD_ALL, FADE_DELAY);

void setup()  
{ 
  Serial.println(SN); 
  //gw.begin(incomingMessage, AUTO, false, 0);
  gw.begin(incomingMessage, AUTO, false, 3);
  
  // Register the LED Dimmable Light with the gateway
  gw.present(CHILD_BLUE, S_DIMMER);
  gw.present(CHILD_RED, S_DIMMER);
  gw.present(CHILD_GREEN, S_DIMMER);
  gw.present(CHILD_ALL, S_DIMMER);
  
  gw.sendSketchInfo(SN, SV);
  // Pull the gateway's current dim level - restore light level upon sendor node power-up
  gw.request(CHILD_BLUE, V_DIMMER);
  gw.request(CHILD_RED, V_DIMMER);
  gw.request(CHILD_ALL, V_DIMMER);
}

/***
 *  Dimmable LED main processing loop 
 */
void loop() 
{
  gw.process();
  ledBlue.Update();
  ledRed.Update();
  ledGreen.Update();
  ledAll.Update();
}

void incomingMessage(const MyMessage &message) {
  Serial.println("In incomingMessage");
  Serial.println(message.data);
  Serial.println(message.type);
  Serial.println(message.sensor);
  
  if (message.type == V_LIGHT || message.type == V_DIMMER) {
    //  Retrieve the power or dim level from the incoming request message
    int requestedLevel = atoi( message.data );
    
    // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
    requestedLevel *= ( message.type == V_LIGHT ? 100 : 1 );
    
    // Clip incoming level to valid range of 0 to 100
    requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
    requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;
    if(message.sensor == 0) { //Blue
      ledBlue.SetDesiredLedLevel(requestedLevel);
    }
    else if(message.sensor == 1) { //Red
      ledRed.SetDesiredLedLevel(requestedLevel);
    }
    else if(message.sensor == 2) { //Green
      ledGreen.SetDesiredLedLevel(requestedLevel);
    }
    else if(message.sensor == 3) { //CHILD_ALL
      //Get the highest value to work with
      int highest = ledBlue.GetCurrentLedLevel();
      if(ledRed.GetCurrentLedLevel() > highest){
        highest = ledRed.GetCurrentLedLevel();
      }
      if(ledGreen.GetCurrentLedLevel() > highest){
        highest = ledGreen.GetCurrentLedLevel();        
      }
      
      //Calculate the percentage to change all the leds
      double percentToChange = (double)requestedLevel / (double)highest;
      
      //If no led is on, try to restore the last led value
      if(highest == 0){
        ledBlue.RestoreLastLedLevel();
        ledRed.RestoreLastLedLevel();
        ledGreen.RestoreLastLedLevel();
      }
      else if(percentToChange > 0){
        ledBlue.ChangeLevelInPercent(percentToChange);
        ledRed.ChangeLevelInPercent(percentToChange);
        ledGreen.ChangeLevelInPercent(percentToChange);
      }
      else{
        ledBlue.SetDesiredLedLevel(requestedLevel);
        ledRed.SetDesiredLedLevel(requestedLevel);
        ledGreen.SetDesiredLedLevel(requestedLevel);
      }
      
      Serial.print("requestedLevel ");
      Serial.print(requestedLevel);
      Serial.print(" highest ");
      Serial.print(highest);
      Serial.print(" percent change is ");
      printDouble(percentToChange, 4);
      Serial.println();
      
      ledAll.SetDesiredLedLevel(requestedLevel);
      ledAll.ForceGwUpdate();
    }
  }
}

void printDouble(double val, byte precision) {
  Serial.print (int(val));                                     // Print int part
  if( precision > 0) {                                         // Print decimal part
    Serial.print(".");
    unsigned long frac, mult = 1;
    byte padding = precision -1;
    while(precision--) mult *=10;
    if(val >= 0) frac = (val - int(val)) * mult; else frac = (int(val) - val) * mult;
    unsigned long frac1 = frac;
    while(frac1 /= 10) padding--;
    while(padding--) Serial.print("0");
    Serial.print(frac,DEC) ;
  }
} 
