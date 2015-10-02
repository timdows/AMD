#include <SPI.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "LedControl.h"
#include "Character.h"

SoftwareSerial ESPserial(2, 3); // RX, TX
LedControl lc=LedControl(7, 9, 8, 2); //DataIn, CLK, LOAD

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
String request = "getData()";
unsigned long timeout = 5000;
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

  char emptyArray[4] = {'1', '2', '3', '4'};
  writeSevenSegment(emptyArray);

  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  //Set the brightness to a medium values
  lc.setIntensity(0,8);
  lc.setIntensity(1,8);
  //and clear the display
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  testBelowSegments();
}

void loop(){
  if(millis() - previousMain > timeoutMain){
    previousMain = millis();
    makeRequest();
  }
}

void testBelowSegments(){
  lc.setDigit(0, 0, 5, false);
  lc.setDigit(0, 1, 6, true);
  lc.setDigit(1, 2, 7, false);
  lc.setDigit(1, 3, 8, true);
}

void makeRequest(){
  flushESPserial();

  incomming = "";
  capturing = false;
  
  unsigned long started = millis();
  ESPserial.println(request);

  bool canRun = true;

  while(millis() - started < timeout && canRun){
    while(ESPserial.available() > 0 && canRun){
      char c = ESPserial.read();
      if(c == '{'){
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

    //Get the watt usage and display on big seven segment displays
    const char* usage = root["watt"]; //Get the watt variable from json object
    char usageArray[4] = {'b'}; //Fill with empty data (b = 11 = empty according to fontArray)
    int usageLength = strlen(usage);

    //Serial.print("Usage usageLength ");
    //Serial.println(usageLength);
    
    for(int i = 0; i < usageLength; i++){
      int place = 4 - usageLength + i;
      
      Serial.print("Char at position ");
      Serial.print(place);
      Serial.print(" has value ");
      char c = usage[i];
      Serial.println(c);
      usageArray[place] = c;
    }
    writeSevenSegment(usageArray);

    //Get the totals and display on small seven segment displays
    const char* lastWeekTotalUsage = root["lastWeekTotal"]; //Get the variable from json object
    Character lastWeekTotalChars[4];
    int lastWeekTotalUsageLength = strlen(lastWeekTotalUsage);

    int dots = 0;
    for(int i = 0; i < lastWeekTotalUsageLength; i++){    
      char c = lastWeekTotalUsage[i];
      int n = charToInt(c);

      //If it is a dot
      if(n == 100){
        dots++;
        lastWeekTotalChars[i - dots].setDot();
      }
      else{
        lastWeekTotalChars[i - dots].setNumber(charToInt(c));
      }
    }
    writeSmallSevenSegment(0, 0, lastWeekTotalChars);    

    //Get the totals and display on small seven segment displays
    const char* thisWeekTotalUsage = root["thisWeekTotal"]; //Get the variable from json object
    Character thisWeekTotalChars[4];
    int thisWeekTotalUsageLength = strlen(thisWeekTotalUsage);

    dots = 0;
    for(int i = 0; i < thisWeekTotalUsageLength; i++){    
      char c = thisWeekTotalUsage[i];
      int n = charToInt(c);

      //If it is a dot
      if(n == 100){
        dots++;
        thisWeekTotalChars[i - dots].setDot();
      }
      else{
        thisWeekTotalChars[i - dots].setNumber(charToInt(c));
      }
    }
    writeSmallSevenSegment(0, 1, thisWeekTotalChars);   

    //Get the totals and display on small seven segment displays
    const char* lastMonthTotalUsage = root["lastMonthTotal"]; //Get the variable from json object
    Character lastMonthTotalChars[4];
    int lastMonthTotalUsageLength = strlen(lastMonthTotalUsage);

    dots = 0;
    for(int i = 0; i < lastMonthTotalUsageLength; i++){    
      char c = lastMonthTotalUsage[i];
      int n = charToInt(c);

      //If it is a dot
      if(n == 100){
        dots++;
        lastMonthTotalChars[i - dots].setDot();
      }
      else{
        lastMonthTotalChars[i - dots].setNumber(charToInt(c));
      }

      //Canot display more than 4 characters on the screen
      if(i - dots > 3){
        break;
      }
    }
    writeSmallSevenSegment(1, 0, lastMonthTotalChars);   

    //Get the totals and display on small seven segment displays
    const char* thisMonthTotalUsage = root["thisMonthTotal"]; //Get the variable from json object
    Character thisMonthTotalChars[4];
    int thisMonthTotalUsageLength = strlen(thisMonthTotalUsage);

    dots = 0;
    for(int i = 0; i < thisMonthTotalUsageLength; i++){    
      char c = thisMonthTotalUsage[i];
      int n = charToInt(c);

      //If it is a dot
      if(n == 100){
        dots++;
        thisMonthTotalChars[i - dots].setDot();
      }
      else{
        thisMonthTotalChars[i - dots].setNumber(charToInt(c));
      }

      //Canot display more than 4 characters on the screen
      if(i - dots > 3){
        break;
      }
    }
    writeSmallSevenSegment(1, 1, thisMonthTotalChars);   
  }
}

//TODO changek argument chars to pointer
void writeSmallSevenSegment(int address, int display, Character chars[]){
  int displayAdd = 0;
  if(display == 1){
    displayAdd = 4;
  }
  
  lc.setDigit(address, 3 + displayAdd, chars[0].number, chars[0].dot);
  lc.setDigit(address, 2 + displayAdd, chars[1].number, chars[1].dot);
  lc.setDigit(address, 1 + displayAdd, chars[2].number, chars[2].dot);
  lc.setDigit(address, 0 + displayAdd, chars[3].number, chars[3].dot);
}

//TODO change argument array to pointer
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
    case '.':
    case ',':
      return 100;
      
  }
}

void flushESPserial(){
  while(ESPserial.available() > 0) {
    ESPserial.read();
  }
}
