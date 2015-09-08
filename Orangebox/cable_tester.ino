#include <Adafruit_NeoPixel.h>

#define LED_PIN 11
#define INPUT_PIN 2
int MUX_LEFT[] = {3, 4, 6, 8};
int MUX_RIGHT[] = {5, 7, 9, 10};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, LED_PIN, NEO_GRB + NEO_KHZ800);

int buttonState = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting ORANGEBOX");
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  colorWipe(strip.Color(10, 10, 10), 50); // start ?
  
  pinMode(MUX_LEFT[0], OUTPUT);
  pinMode(MUX_LEFT[1], OUTPUT);
  pinMode(MUX_LEFT[2], OUTPUT);
  pinMode(MUX_LEFT[3], OUTPUT);
  pinMode(MUX_RIGHT[0], OUTPUT);
  pinMode(MUX_RIGHT[1], OUTPUT);
  pinMode(MUX_RIGHT[2], OUTPUT);
  pinMode(MUX_RIGHT[3], OUTPUT);

  pinMode(INPUT_PIN, INPUT);
  
  //delay(1000);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void loop() {
  //Serial.println("--------------------------------");
  
  //Set all leds off
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
        Serial.print("Got connection on  ");
        Serial.print(countLeft);
        Serial.print(" - ");
        Serial.println(countRight);
      }
      
      if(connected && alreadyConnected != -1){
        Serial.print("already connected before on ");
        Serial.println(alreadyConnected);
      }

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
}

int checkAlreadyConnected(boolean val[16][16], int count){
  for(int i = 0; i < 16; i++){
    if(val[i][count] == true){
      return i;
    }
  }
  return -1;
}

/*
void setLedDefinedColor(int ledNumber, int r, int g, int b){
  strip.setPixelColor(ledNumber, r, g, b);
}

void setLedColor(int ledNumber, uint32_t color){
  strip.setPixelColor(ledNumber, color);
}*/

void setLedsOff(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0, 0, 0);
  }
}

void setLedByIntLeft(int count, uint32_t color){
  switch(count){
    case 0: strip.setPixelColor(23, color); break;
    case 1: strip.setPixelColor(22, color); break;
    case 2: strip.setPixelColor(21, color); break;
    case 7: strip.setPixelColor(20, color); break;
    case 6: strip.setPixelColor(19, color); break;
    case 3: strip.setPixelColor(18, color); break;
    case 4: strip.setPixelColor(17, color); break;
    case 5: strip.setPixelColor(16, color); break;
    case 8: strip.setPixelColor(24, color); break;
    
    /*
    case 9: strip.setPixelColor(15, color); break;
    case 10: strip.setPixelColor(15, color); break;
    case 11: strip.setPixelColor(15, color); break;
    case 12: strip.setPixelColor(15, color); break;
    case 13: strip.setPixelColor(15, color); break;
    case 14: strip.setPixelColor(15, color); break;
    case 15: strip.setPixelColor(15, color); break;
    */
  }
}

void setLedByIntRight(int count, uint32_t color){
  switch(count){
    case 0: strip.setPixelColor(15, color); break;
    case 1: strip.setPixelColor(14, color); break;
    case 2: strip.setPixelColor(13, color); break;
    case 7: strip.setPixelColor(12, color); break;
    case 6: strip.setPixelColor(11, color); break;
    case 3: strip.setPixelColor(10, color); break;
    case 4: strip.setPixelColor(9, color); break;
    case 5: strip.setPixelColor(8, color); break;
    case 8: strip.setPixelColor(0, color); break;
    
    /*
    case 9: strip.setPixelColor(10, color); break;
    case 10: strip.setPixelColor(11, color); break;
    case 11: strip.setPixelColor(12, color); break;
    case 12: strip.setPixelColor(13, color); break;
    case 13: strip.setPixelColor(14, color); break;
    case 14: strip.setPixelColor(15, color); break;
    case 15: strip.setPixelColor(0, color); break;
    */
  }
}

uint32_t getSingleColor(int i){
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
}
