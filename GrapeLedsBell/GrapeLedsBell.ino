#include "Adafruit_WS2801.h"
#include "SPI.h"

//Stepper setup
int dirpin = 3;
int steppin = 2;

int speedForward = 300;
int speedBackward = 3000;

int forwardSteps = 500;
int backwardSteps = 500;

//Led setup
uint8_t dataPin  = 5;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 4;    // Green wire on Adafruit Pixels

Adafruit_WS2801 strip = Adafruit_WS2801(12, dataPin, clockPin);

unsigned long previousMillis;
int currentRainbowCycle = 0;

//Button setup
int buttonpin = 7;

void setup(){
  pinMode(buttonpin, INPUT_PULLUP);
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);

  strip.begin();
}

void loop(){
  if (digitalRead(buttonpin) == LOW) {
    runProgram();
  }

  rainbowCycle();
}

void runProgram(){
  colorWipe(Color(0, 255, 0), 50);

  delay(4000);

  //start when it is touching the bell and go backward
  setDirectionBackward();
  move(speedBackward, backwardSteps);

  //ring for 11 times
  for(int i = 0; i < 11; i++){
    strip.setPixelColor(i, Color(0, 0, 255));
    strip.show();
    ring();
  }

  //ring the 12th time faster
  setDirectionForward();
  move(speedForward, forwardSteps);

  strip.setPixelColor(11, Color(0, 0, 255));
  strip.show();
  
  for(int i = 0; i < 5; i++){
    finishRing();
  }

  delay(15000);
}

void ring(){
  setDirectionForward();
  move(speedForward, forwardSteps);
  //delay(100);
  setDirectionBackward();
  move(speedBackward, backwardSteps);
  delay(1600);
}

void finishRing(){
  setDirectionBackward();
  move(speedBackward, 49);
  //delay(10);
  setDirectionForward();
  move(speedForward, 50);
  delay(10);
}

void setDirectionForward(){
  digitalWrite(dirpin, HIGH);
}

void setDirectionBackward(){
  digitalWrite(dirpin, LOW);
}

void move(int speed, int steps){
  for (int i = 0; i < steps; i++){
    digitalWrite(steppin, LOW);
    digitalWrite(steppin, HIGH); 
    delayMicroseconds(speed);     
  } 
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait){
  int i;
  for (i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbowCycle() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 10) {
    previousMillis = currentMillis;  // Remember the time
    currentRainbowCycle++;

    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + currentRainbowCycle) & 255));
    }

    strip.show();

    if (currentRainbowCycle == (256 * 5)) {
      currentRainbowCycle = 0;
    }
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b){
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
