/*
 * WS2811 class
 */
#include "WS2801.h"

//Constructor
WS2811::WS2811(MySensor *mySensorGw){
  this->mySensorGw = mySensorGw;
  
  for(int i = 0; i < NUMBER_OF_SCENES; i++){
    this->sceneStatus[i] = false;
  }
  
  this->strip.begin();
  //this->strip.show(); // Initialize all pixels to 'off', but a bug that it cannot be enabled??
}

void WS2811::Update(){
  if(this->sceneStatus[0] == true){
    this->KnightRider();
  }
  else if(this->sceneStatus[1] == true){
    this->RainbowCycle();
  }
  else if(this->sceneStatus[2] == true){
    if(this->currentFullRGB == false){
      this->FullRGBColor(true);
    }
  }   
  else if(this->sceneStatus[3] == true){
    this->Disco();
  }
}

void WS2811::ProcessScene(int sensor, int toValue){
  int sceneNumber = sensor - 1;
  //int sceneNumber = sensor;
  
  //Set all to false
  for (int i = 0; i < NUMBER_OF_SCENES; i++) {
    Serial.print("sceneStatus details ");
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(this->sceneStatus[i]);
    
    if(this->sceneStatus[i] == true && i != sceneNumber){
      this->sceneStatus[i] = false;

      //Send to gateway that what is on is now off
      //Make sure that scene is i+1 (=sensor)
      Serial.print("turning off sensor ");
      Serial.println(i);
      this->mySensorGw->send(MyMessage(i + NUMBER_OF_SCENES + 1, V_LIGHT).set(0));
    }
  }
  
  //Set our scene to true
  Serial.print("Changing scene ");
  Serial.print(sceneNumber);
  Serial.print(" to value ");
  Serial.println(toValue);
  this->sceneStatus[sceneNumber] = toValue;
  
  //If all scenes are off, turn off the lights as well
  boolean sceneRunning = false;
  for (int i = 0; i < NUMBER_OF_SCENES; i++) {
    sceneRunning = this->sceneStatus[i];
  }
  
  if(!sceneRunning){
    this->FullRGBColor(false);
  }
}

void WS2811::ColorWipe(uint32_t color) {
  if(this->currentColorWipe < this->strip.numPixels()){
    this->currentColorWipe++;
    this->strip.setPixelColor(this->currentColorWipe, color);
    strip.show();
  }
}

/*
*  KnightRider
*/
void WS2811::KnightRider(){
  unsigned long currentMillis = millis();
  
  if(currentMillis - this->previousMillis >= this->delayKnightRider){
    this->previousMillis = currentMillis;  // Remember the time
    
    uint32_t colorOn;
    if(this->greenRGB == 0 && this->redRGB == 0 && this->blueRGB == 0){
      colorOn = this->strip.Color(0, 255, 0);
    }
    else{
      colorOn = this->strip.Color(
        (int)(this->greenRGB / 100. * 255), 
        (int)(this->redRGB / 100. * 255), 
        (int)(this->blueRGB / 100. * 255)
      );
    }
    
    uint32_t colorOff = this->strip.Color(0, 0, 0);
    
    if(this->currentKnightRider < this->strip.numPixels()){
      this->currentKnightRider++;
      this->strip.setPixelColor(this->currentKnightRider, colorOn);
      
      //Remove after 5 lights
      int toRemove = this->currentKnightRider - 10;
      if(toRemove < 0){
        toRemove = this->strip.numPixels() + toRemove;
      }
      this->strip.setPixelColor(toRemove, colorOff);
      strip.show();
    }
    else{
      this->currentKnightRider = 0;
    }
  }
}

/*
*  Slightly different, this makes the rainbow equally distributed throughout
*/
void WS2811::RainbowCycle() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - this->previousMillis >= this->delayRainbowCycle){
    this->previousMillis = currentMillis;  // Remember the time
    this->currentRainbowCycle++;
      
    for(int i = 0; i < this->strip.numPixels(); i++) {
      this->strip.setPixelColor(i, this->Wheel(((i * 256 / this->strip.numPixels()) + this->currentRainbowCycle) & 255));
    }

    strip.show();
    
    if(this->currentRainbowCycle == (256*5)){
      this->currentRainbowCycle = 0;
    }
  }
}

/*
*  Get color for RGB values, and set full ledstrip to this color
*/
void WS2811::FullRGBColor(boolean state){
  uint32_t color;
  
  //Turn them on
  if(state){
    this->currentFullRGB = true;
    color = this->strip.Color(
      (int)(this->greenRGB / 100. * 255), 
      (int)(this->redRGB / 100. * 255), 
      (int)(this->blueRGB / 100. * 255)
    );
  }
  //Turn them off
  else{
    this->currentFullRGB = false;
    color = 0;
  }
  
      
  for(int i = 0; i < this->strip.numPixels(); i++) {
    this->strip.setPixelColor(i, color);
  }

  strip.show();
}

/*
*
*/
void WS2811::Disco(){
  unsigned long currentMillis = millis();
  
  if(currentMillis - this->previousMillis >= this->delayDisco){
    this->previousMillis = currentMillis;  // Remember the time
    
    //Turn all leds off
    for(int i = 0; i < this->strip.numPixels(); i++) {
      this->strip.setPixelColor(i, 0);
    }
    
    uint32_t color = this->strip.Color(
      (int)(this->greenRGB / 100. * 255), 
      (int)(this->redRGB / 100. * 255), 
      (int)(this->blueRGB / 100. * 255)
    );
    
    //Control the amount of leds going on
    for(int i = 0; i < 5; i++){
      int randomLed = random(1, NUM_LEDS);      
      this->strip.setPixelColor(randomLed, color);
    }
    
    strip.show();
  }
}

/*
*  Part for updating RGB values
*/
void WS2811::SetRGBValue(const MyMessage &message){
    //  Retrieve the power or dim level from the incoming request message
    int requestedLevel = atoi( message.data );
    
    // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
    requestedLevel *= ( message.type == V_LIGHT ? 100 : 1 );
    
    // Clip incoming level to valid range of 0 to 100
    requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
    requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;
    
    if(message.sensor == 0) {
      this->redRGB = requestedLevel;
    }
    else if(message.sensor == 1) {
      this->greenRGB = requestedLevel;
    }
    else if(message.sensor == 2) { 
      this->blueRGB = requestedLevel;
    }
    else if(message.sensor == 3) { //all
      //Get the highest value to work with
      int highest = this->redRGB;
      if(this->greenRGB > highest){
        highest = this->greenRGB;
      }
      if(this->blueRGB > highest){
        highest = this->blueRGB;       
      }
      
      //Calculate the percentage to change all the leds
      double percentToChange = (double)requestedLevel / (double)highest;
      
      if(highest == 0){
        this->redRGB = requestedLevel;
        this->greenRGB = requestedLevel;
        this->blueRGB = requestedLevel;
      }
      else if(percentToChange > 0){
        this->redRGB = floor(this->redRGB  * percentToChange);
        this->greenRGB = floor(this->greenRGB * percentToChange);
        this->blueRGB = floor(this->blueRGB * percentToChange);
      }
      //Never gets here?
      else{
        this->redRGB = requestedLevel;
        this->greenRGB = requestedLevel;
        this->blueRGB = requestedLevel;
      }
      
      Serial.print("requestedLevel ");
      Serial.print(requestedLevel);
      Serial.print(" highest ");
      Serial.print(highest);
      Serial.print(" percent change is ");
      this->PrintDouble(percentToChange, 4);
      Serial.println();
      
      //ledAll.SetDesiredLedLevel(requestedLevel);
      //ledAll.ForceGwUpdate();*/
      
      //Update RGB values via gateway
      this->mySensorGw->send(MyMessage(0, V_DIMMER).set(this->redRGB));
      this->mySensorGw->send(MyMessage(1, V_DIMMER).set(this->greenRGB));
      this->mySensorGw->send(MyMessage(2, V_DIMMER).set(this->blueRGB));
    }
    
    //Make sure to update other running scenes
    this->currentFullRGB = false;
}

void WS2811::PrintDouble(double val, byte precision) {
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t WS2811::Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return this->strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return this->strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return this->strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
