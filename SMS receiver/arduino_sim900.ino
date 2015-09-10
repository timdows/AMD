HardwareSerial & dbgTerminal = Serial;

void setup(){
  ledSetup();     
  dbgTerminal.begin(9600); // Serial monitor for debugging
  espSetup(); //Wifi
  simSetup(); //GSM
}

void loop(){
  espLoop();
  simLoop();  
}












