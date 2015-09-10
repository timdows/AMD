#include <SoftwareSerial.h>

//SoftwareSerial SIM900(10, 11); // configure software serial port
HardwareSerial & SIM900 = Serial2;
const int SIM900_PWR = 9;

String receivedSmsses[5];
int currentSmsCount = 0;
String currentSmsText = "";
boolean receivedSms = false;

void simSetup(){
  SetSimLedToColor(200, 0 ,0);
  pinMode(SIM900_PWR, OUTPUT);
  SIM900.begin(19200);
  
  dbgTerminal.println("SIM900 starting...");
  SIM900power();  
  
  CheckModemFunctioning();
  
  //callSomeone();
  //setSmsSettings();
}

int GetCurrentSmsCount(){
  return currentSmsCount;
}

String GetSmsFromArray(int i){
  return receivedSmsses[i];
}

void simLoop(){
  while(SIM900.available()) {
    receivedSms = true;
    char c = SIM900.read(); //Get the character from the cellular serial port.
    currentSmsText += c;
  }
  if(receivedSms){
    receivedSms = false;
    receivedSmsses[currentSmsCount] = currentSmsText;
    currentSmsText = "";
    currentSmsCount++;
  }
  
  //MapSimToDebugTerminal();
}

// software equivalent of pressing the GSM shield "power" button
void SIM900power(){
  SetSimLedToColor(200, 50 ,0);
  digitalWrite(SIM900_PWR, HIGH);
  delay(1000);
  digitalWrite(SIM900_PWR, LOW);
  delay(3000);
  
  SIM900.println("AT");
  if(FindSimResponse()){
    dbgTerminal.println("Started SIM900");
    SetSimLedToColor(0, 50 ,100);
  }
  else{
    SetSimLedToColor(30, 50 ,0);
    dbgTerminal.println("Failed to start SIM900, retry");
    delay(500);
    SIM900power();
  }  
}

void setSmsSettings(){
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
}
 
void callSomeone(){
  SetSimLedToColor(0, 100, 100);
  SIM900.println("ATD + +xxxxxxxxxxxx;");
  //SIM900.println("ATD1244;");
  delay(100);
  SIM900.println();
  delay(15000);            // wait for x seconds...
  SIM900.println("ATH");   // hang up
  SetSimLedToColor(0, 0, 100);
}
 
void CheckModemFunctioning(){
  dbgTerminal.print("Check SIM900 connection: ");
  dbgTerminal.println(GetSIMResponse("AT", 100));
  dbgTerminal.print("Check SIM900 manu: ");
  dbgTerminal.println(GetSIMResponse("ATI", 100));
  dbgTerminal.print("Check SIM900 gsv: ");
  dbgTerminal.println(GetSIMResponse("AT+GSV", 100));
}

String GetSIMResponse(String AT_Command, int wait){
  String tmpData;
  
  SIM900.println(AT_Command);
  //SIM900.print("AT\r\n");
  delay(wait);
  while (SIM900.available() >0 )  {
    char c = SIM900.read();
    tmpData += c;
    
    //if ( tmpData.indexOf(AT_Command) > -1 )         
    //  tmpData = "";
    //else
    //  tmpData.trim();       
          
   }
   return tmpData;
}

void MapSimToDebugTerminal(){
  if (SIM900.available()){
    SetSimLedToColor(0, 50 ,0);
    dbgTerminal.write(SIM900.read());
  }
  if (Serial.available()){
    SetSimLedToColor(0, 50 ,50);
    char c = Serial.read();
    SIM900.write(c);
    Serial.write(c);
  }
}

boolean FindSimResponse(){
  String tmpData;
  int count = 0;
  
  while(count < 20){
    while (SIM900.available() > 0 ){
      char c = SIM900.read();
      tmpData += c;
      dbgTerminal.print(tmpData);
      
      if(tmpData.indexOf("OK") != -1 ){
        return true;
      }
      if(tmpData.indexOf("FAIL") != -1 ){
        return false;
      }
      if(tmpData.indexOf("NORMAL POWER DOWN") != -1){
        return false;
      }
    }
    delay(500);
    dbgTerminal.print(".");
    tmpData = "";
    count++;
  }
  return false;
}
