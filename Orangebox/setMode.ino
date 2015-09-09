
void setModeOnLeds(int mode){
  setLedsOff();
  uint32_t color = strip.Color(20, 20, 20);

  byte stripArray[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
  };
  
  switch(mode){
    case 0: //DB15 to DB15
      stripArray[2] = 0b11111110;
      stripArray[3] = 0b11111111;
      stripArray[1] = 0b01111111;
      stripArray[0] = 0b11111111;
      break;
    case 1: //DB15 to DB9
      stripArray[2] = 0b11111110;
      stripArray[3] = 0b11111111;
      stripArray[1] = 0b00001111;
      stripArray[0] = 0b11111000;
      break;
    case 2: //DB15 to RJ45
      stripArray[2] = 0b11111110;
      stripArray[3] = 0b11111111;
      stripArray[1] = 0b00000000;
      stripArray[0] = 0b11111111;
      break;
      
    case 3: //DB9 to DB15
      stripArray[2] = 0b00011110;
      stripArray[3] = 0b11111000;
      stripArray[1] = 0b01111111;
      stripArray[0] = 0b11111111;
      break;
    case 4: //DB9 to DB9
      stripArray[2] = 0b00011110;
      stripArray[3] = 0b11111000;
      stripArray[1] = 0b00001111;
      stripArray[0] = 0b11111000;
      break;
    case 5: //DB9 to RJ45
      stripArray[2] = 0b00011110;
      stripArray[3] = 0b11111000;
      stripArray[1] = 0b00000000;
      stripArray[0] = 0b11111111;
      break;
      
    case 6: //RJ45 to DB15
      stripArray[2] = 0b00000000;
      stripArray[3] = 0b11111111;
      stripArray[1] = 0b01111111;
      stripArray[0] = 0b11111111;
      break;
    case 7: //RJ45 to DB9
      stripArray[2] = 0b00000000;
      stripArray[3] = 0b11111111;
      stripArray[1] = 0b00001111;
      stripArray[0] = 0b11111000;
      break;
    case 8: //RJ45 to RJ45
      stripArray[2] = 0b00000000;
      stripArray[3] = 0b11111111;
      stripArray[1] = 0b00000000;
      stripArray[0] = 0b11111111;
      break;
  }

  for(int i = 0; i < sizeof(stripArray); i++){    
    for(int j = 0; j < 8; j++){
      int stripNumber = i * 8 + j;
      if(bitRead(stripArray[i], j) == 1){
        strip.setPixelColor(stripNumber, color);
      }
      else{
        strip.setPixelColor(stripNumber, 0, 0, 0);
      }
    }
  }
  strip.show();
  //delay(1000);
}

