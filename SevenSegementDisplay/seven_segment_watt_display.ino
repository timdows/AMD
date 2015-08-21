#include <SPI.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "LedControl.h"

SoftwareSerial ESPserial(2, 3); // RX, TX
LedControl lc=LedControl(7,9,8,1); //DataIn, CLK, LOAD

byte fontArray[] = {
  // dp-a-b-c-d-e-f-g
  0b11101110, // 0
  0b00101000, // 1
  0b11001101, // 2
  0b01101101, // 3
  0b00101011, // 4
  0b01100111, // 5
  0b11100111, // 6
  0b00101100, // 7
  0b11101111, // 8
  0b01101111, // 9
  0b00010000, // DP = index10
  0b00000000, //empty = index11
};

//Pin connected to Data in SER IN pin 2 of 6c595
const int dataPin = 11;
//Pin connected to latch pin (SRCK)pin 15 of 6c595
const int latchPin = 13;
//Pin connected to clock pin (RCK) pin 10 of 6c595 as the slave select
const int slaveSelectPin = 10;

const int espCHPD = 4;
String incomming = "";
String request = "38TTT";
unsigned long timeout = 2500;
bool capturing = false;

unsigned long previousMain = 0;
unsigned long timeoutMain = 10000;

int timesFailed = 0;

void setup() {
  // set the Pins as output:
  pinMode(dataPin, OUTPUT); 
  pinMode(latchPin, OUTPUT);
  pinMode (slaveSelectPin, OUTPUT);
  pinMode (espCHPD, OUTPUT);
  
  Serial.begin(9600);
  ESPserial.begin(9600);

  digitalWrite(espCHPD, LOW);
  delay(100);
  digitalWrite(espCHPD, HIGH);
  
  SPI.begin();
  digitalWrite(slaveSelectPin,LOW); // take the SS pin low to select the chip:

  //char emptyArray[4] = {'b'};
  char emptyArray[4] = {'1', '2', '3', '4'};
  writeSevenSegment(emptyArray);

  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0,false);
  //Set the brightness to a medium values
  lc.setIntensity(0,8);
  //and clear the display
  lc.clearDisplay(0);
}

void loop(){
  if(millis() - previousMain > timeoutMain){
    previousMain = millis();
    makeRequest();
  }
  scrollDigits();
}

void scrollDigits() {
  for(int i=0;i<13;i++) {
    lc.setDigit(0,7,i,false);
    lc.setDigit(0,6,i+1,false);
    lc.setDigit(0,5,i+2,false);
    lc.setDigit(0,4,i+3,false);
    lc.setDigit(0,3,i+4,false);
    lc.setDigit(0,2,i+5,false);
    lc.setDigit(0,1,i+6,false);
    lc.setDigit(0,0,i+7,false);
    delay(250);
  }
  //lc.clearDisplay(0);
  delay(250);
}

void makeRequest(){
  flushESPserial();

  incomming = "";
  capturing = false;
  
  unsigned long started = millis();
  ESPserial.println(request);

  int countOpeningBrackets = 0;
  bool canRun = true;

  while(millis() - started < timeout && canRun){
    while(ESPserial.available() > 0 && canRun){
      char c = ESPserial.read();
      if(c == '{'){
        countOpeningBrackets++;
      }

      if(countOpeningBrackets == 3){
        capturing = true;
      }

      if(capturing){
        incomming += c;

        if(c == '}'){
          capturing = false;
          canRun = false;
          break;
        }
      }
    }
  }

  Serial.println(incomming);

  int len = incomming.length() + 1;
  StaticJsonBuffer<250> jsonBuffer;
  char json[len];
  incomming.toCharArray(json, len); 
  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success()) {
    Serial.println("Parsing of json failed");
    timesFailed++;

    //Reset the ESP via the transistor
    if(timesFailed == 3){
      timesFailed = 0;
      digitalWrite(espCHPD, LOW);
      delay(100);
      digitalWrite(espCHPD, HIGH);
    }
  }
  else{
    timesFailed = 0;
    const char* usage = root["value"];
    char usageArray[4] = {'b'};
    int usageLength = strlen(usage);

    //Serial.print("Usage usageLength ");
    //Serial.println(usageLength);
    
    for(int i = 0; i < usageLength; i++){
      int place = 4 - usageLength + i;
      
      //Serial.print("Char at position ");
      //Serial.print(place);
      //Serial.print(" has value ");
      char c = usage[i];
      //Serial.println(c);
      usageArray[place] = c;
    }

    writeSevenSegment(usageArray);
  }
  
}

void writeSevenSegment(char array[]){
  digitalWrite (slaveSelectPin, LOW); // << RCLK line goes low
  SPI.transfer (fontArray[charToInt(array[0])]);  //  << SRCLK goes  high-low 8 times to output 8 bits of data
  digitalWrite (slaveSelectPin, HIGH);
  delay(150);
  digitalWrite (slaveSelectPin, LOW);
  SPI.transfer (fontArray[charToInt(array[2])]);
  digitalWrite (slaveSelectPin, HIGH);
  delay(150);
  digitalWrite (slaveSelectPin, LOW);
  SPI.transfer (fontArray[charToInt(array[3])]);
  digitalWrite (slaveSelectPin, HIGH);
  delay(150);
  digitalWrite (slaveSelectPin, LOW);
  SPI.transfer (fontArray[charToInt(array[1])]);
  digitalWrite (slaveSelectPin, HIGH); // data outputs change on this rising edge << RCLK line goes high to move data into output register
}

int charToInt(char c){
  switch(c){
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'a':
      return 10;
    case 'b':
      return 11;
  }
}

void flushESPserial(){
  while(ESPserial.available() > 0) {
    ESPserial.read();
  }
}
