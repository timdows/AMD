#include <MySensor.h>
#include <SPI.h>
#include "WS2801.h"
//#include "ACS712.h"

#define NUMBER_OF_SCENES 4

#define CHILD_RED 0
#define CHILD_GREEN 1
#define CHILD_BLUE 2
#define CHILD_ALL 3

MySensor gw;
WS2811 ledStrip(&gw);
//ACS712 acs712(&gw);

void setup(){  
  // Initialize library and add callback for incoming messages
  gw.begin(incomingMessage, AUTO, false, 3);
  //Serial.println("test");
  
  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("TV Meubel", "1.0");

  // Register the LED Dimmable Light with the gateway
  gw.present(CHILD_RED, S_DIMMER);
  gw.present(CHILD_GREEN, S_DIMMER);  
  gw.present(CHILD_BLUE, S_DIMMER);
  gw.present(CHILD_ALL, S_DIMMER);

  // Register and get what scene is on
  for (int scene = 1; scene <= NUMBER_OF_SCENES; scene++) {
    // Register all scenes to gw (they will be created as child devices)
    gw.present(scene + 4, S_LIGHT);
  }
}


void loop(){
  // Alway process incoming messages whenever possible
  gw.process();
  
  ledStrip.Update();
  
  //acs712.CheckPowerUsage();
}

void incomingMessage(const MyMessage &message){
  Serial.println(message.data);
  Serial.println(message.type);
  Serial.println(message.sensor);
 
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type == V_LIGHT || message.type == V_DIMMER) {
    //Always force an update of the watts used
    //acs712.ForceUpdate();
    
    //Update RGB values
    if(message.sensor < 4){
      ledStrip.SetRGBValue(message);
    }
    //Updat scene values
    else{
      int toValue = atoi( message.data );
      // Write some debug info
      Serial.print("Incoming change for sensor:");
      Serial.print(message.sensor);
      Serial.print(", New status: ");
      Serial.println(toValue);
      
      //Tell the ledStrip object to process the scene
      ledStrip.ProcessScene(message.sensor - 4, toValue);
    }
  }
}


