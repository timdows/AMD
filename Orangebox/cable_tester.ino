#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

#define LED_PIN 11
#define INPUT_PIN 2
#define MODE_PIN 13

int MUX_LEFT[] = {7, 9, 10, 5};
int MUX_RIGHT[] = {4, 3, 8, 6};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, LED_PIN, NEO_GRB + NEO_KHZ800);

int mode = 5;
Bounce debouncer = Bounce();
unsigned long lastModeChange;
unsigned long lastAction;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ORANGEBOX");
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  debouncer.attach(MODE_PIN);
  debouncer.interval(20);
  
  //colorWipe(strip.Color(10, 10, 10), 50); // start ?
  setModeOnLeds(mode);
  
  lastModeChange = millis();
  lastAction = millis();
  
  pinMode(MUX_LEFT[0], OUTPUT);
  pinMode(MUX_LEFT[1], OUTPUT);
  pinMode(MUX_LEFT[2], OUTPUT);
  pinMode(MUX_LEFT[3], OUTPUT);
  pinMode(MUX_RIGHT[0], OUTPUT);
  pinMode(MUX_RIGHT[1], OUTPUT);
  pinMode(MUX_RIGHT[2], OUTPUT);
  pinMode(MUX_RIGHT[3], OUTPUT);

  pinMode(INPUT_PIN, INPUT);
}

void loop() {
  debouncer.update();
  
  if(debouncer.fell()){
    lastAction = millis();
    
    //Only change the mode if the button is pressed within 2 seconds of the last press
    if(millis() > lastModeChange + 1000){
      lastModeChange = millis();
    }
    else{
      lastModeChange = millis();
      if(++mode == 9){
        mode = 0;
      }
    }
    setModeOnLeds(mode);
  }
  
  if(millis() < lastModeChange + 1000){
    return;
  }

  setLedsOff();
  
  boolean match[16][16];
  for(int i = 0; i < 16; i++){
    for(int j = 0; j < 16; j++){
      match[i][j] = false;
    }
  }

  for(int countLeft = 0; countLeft < 16; countLeft++){
    digitalWrite(MUX_LEFT[0], bitRead(countLeft, 0));
    digitalWrite(MUX_LEFT[1], bitRead(countLeft, 1));
    digitalWrite(MUX_LEFT[2], bitRead(countLeft, 2));
    digitalWrite(MUX_LEFT[3], bitRead(countLeft, 3));
    
    for(int countRight = 0; countRight < 16; countRight++){
      int alreadyConnected = checkAlreadyConnected(match, countRight);
      
      digitalWrite(MUX_RIGHT[0], bitRead(countRight, 0));
      digitalWrite(MUX_RIGHT[1], bitRead(countRight, 1));
      digitalWrite(MUX_RIGHT[2], bitRead(countRight, 2));
      digitalWrite(MUX_RIGHT[3], bitRead(countRight, 3));
      
      boolean connected = digitalRead(INPUT_PIN);
      
      if(connected){
        lastAction = millis();
        /*Serial.print("Got connection on  ");
        Serial.print(countLeft);
        Serial.print(" - ");
        Serial.println(countRight);*/
      }
      
      /*if(connected && alreadyConnected != -1){
        Serial.print("already connected before on ");
        Serial.println(alreadyConnected);
      }*/

      match[countLeft][countRight] = connected;
      
      //light some LEDs
      if(connected && alreadyConnected == -1){
        //new connection
        setLedByIntRight(countRight, getSingleColor(countLeft));
        setLedByIntLeft(countLeft, getSingleColor(countLeft));
      }
      else if(connected && alreadyConnected != -1){
        //add a color to left
        setLedByIntLeft(countLeft, getSingleColor(alreadyConnected));
      }
    }    
  }
  
  //Show the colors
  strip.show();

  //If for 5 seconds there was no action, display the mode and remember to set the lastModeChange as well
  if(millis() > lastAction + 5000){
    lastModeChange = millis();
    lastAction = millis();
    setModeOnLeds(mode);
  }
}

int checkAlreadyConnected(boolean val[16][16], int count){
  for(int i = 0; i < 16; i++){
    if(val[i][count] == true){
      return i;
    }
  }
  return -1;
}

void setLedsOff(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
}

uint32_t getSingleColor(int i){
  switch(mode){
    case 0: //DB15
    case 1:
    case 2:
      switch(i){
        case 0: return strip.Color(30, 0, 0);
        case 1: return strip.Color(0, 30, 0);
        case 2: return strip.Color(0, 0, 30);
        case 3: return strip.Color(30, 30, 0);
        case 4: return strip.Color(30, 0, 30);
        case 5: return strip.Color(50, 0, 25);
        case 6: return strip.Color(30, 30, 50);
        case 7: return strip.Color(50, 25, 0);
        case 8: return strip.Color(30, 0, 50);
        case 9: return strip.Color(30, 50, 50);
        case 10: return strip.Color(50, 100, 0);
        case 11: return strip.Color(0, 100, 50);
        case 12: return strip.Color(50, 100, 50);
        case 13: return strip.Color(50, 0, 100);
        case 14: return strip.Color(0, 50, 100);
        case 15: return strip.Color(50, 50, 100);
      }
    case 3: //DB9 and RJ45
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      switch(i){
        case 10: return strip.Color(30, 0, 0);
        case 8: return strip.Color(0, 30, 0);
        case 9: return strip.Color(0, 0, 30);
        case 6: return strip.Color(30, 30, 0);
        case 7: return strip.Color(40, 0, 30);
        case 12: return strip.Color(50, 0, 25);
        case 13: return strip.Color(30, 30, 50);
        case 0: return strip.Color(70, 25, 0);
        case 1: return strip.Color(30, 0, 50);
      }
  }
}

