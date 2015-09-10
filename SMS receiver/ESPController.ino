HardwareSerial & espSerial = Serial1;
const int ESP8266_CHPD = 27; //hardware reset

int connectionIds[99];
int currentConnectionId = 0;

unsigned long previousMillis;
long interval = 2500;

String espBuffer = "";

void espSetup(){
  SetWifiLedToColor(200, 0 ,0);
  pinMode(ESP8266_CHPD, OUTPUT);
  digitalWrite(ESP8266_CHPD, HIGH);
  espSerial.begin(9600);
  
  dbgTerminal.println("ESP8266 wifi starting...");  
  SetupWifi();
  dbgTerminal.println("ESP8266 program started");

  previousMillis = millis();
}

void espLoop(){
  if(millis() > previousMillis + 5000){
    previousMillis = millis();
    espBuffer = "";
  }

  while(espSerial.available() > 0){
    char c = espSerial.read();
    dbgTerminal.print(c);
    espBuffer += c;

    if(espBuffer.indexOf("All your sms are belong to us") != -1){
      espBuffer = "";
      
      if(GetCurrentSmsCount() > 0){
        for(int j = 0; j < GetCurrentSmsCount(); j++){
          String willSent = "I will sent you sms ";
          int currSms = j + 1;
          willSent += currSms;
          willSent += " out of ";
          willSent += GetCurrentSmsCount();
          willSent += " smsses in my memory\r\n";
          espSerial.println(willSent);
          
          String sms = GetSmsFromArray(j);
          espSerial.println(sms);
        }
      }
      else{
        espSerial.println("No smsses available yet");
      }
    }
    
    if(espBuffer.indexOf("Call Tim") != -1){
      espBuffer = "";
      callSomeone();
    }
  }
}

void HardResetESP(){
  digitalWrite(ESP8266_CHPD,LOW);
  delay(100);
  digitalWrite(ESP8266_CHPD,HIGH);
  delay(100);  
}

void SetupWifi(){
  SetWifiLedToColor(100, 20 ,0);
  unsigned long startTime = millis();
  String tmpData;
  
  while(millis() < startTime + 15000){
    while(espSerial.available() > 0){
      char c = espSerial.read();
      dbgTerminal.print(c);
      tmpData += c;

      if(tmpData.indexOf("Config done") != -1){
        dbgTerminal.println();
        SetWifiLedToColor(0, 100 ,0);
        return;
      }
    }
  }
  
  SetWifiLedToColor(200, 0 ,0);
  dbgTerminal.println("Failed to connected to wifi, retry");
  HardResetESP();
  delay(500);
  SetupWifi();
}
