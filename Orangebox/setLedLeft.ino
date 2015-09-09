void setLedByIntLeft(int count, uint32_t color){
  switch(mode){
    case 0: //DB15
    case 1:
    case 2:
      switch(count){
        case 0: strip.setPixelColor(27, color); break;
        case 1: strip.setPixelColor(26, color); break;
        case 2: strip.setPixelColor(24, color); break;
        case 3: strip.setPixelColor(25, color); break;
        case 4: strip.setPixelColor(23, color); break;
        //case 5: //not connected to mux
        case 6: strip.setPixelColor(21, color); break;
        case 7: strip.setPixelColor(22, color); break;
        case 8: strip.setPixelColor(19, color); break;
        case 9: strip.setPixelColor(20, color); break;
        case 10: strip.setPixelColor(18, color); break;
        case 11: strip.setPixelColor(17, color); break;
        case 12: strip.setPixelColor(29, color); break;
        case 13: strip.setPixelColor(28, color); break;
        case 14: strip.setPixelColor(30, color); break;
        case 15: strip.setPixelColor(31, color); break;
      }
      break;
    case 3: //DB9
    case 4:
    case 5:
      switch(count){
        case 0: strip.setPixelColor(19, color); break;
        case 1: strip.setPixelColor(20, color); break;
        //case 2: strip.setPixelColor(, color); break; //not connected in DB9 mode
        //case 3: strip.setPixelColor(, color); break; //not connected in DB9 mode
        //case 4: strip.setPixelColor(, color); break; //not connected in DB9 mode
        //case 5: //not connected to mux
        case 6: strip.setPixelColor(28, color); break;
        case 7: strip.setPixelColor(27, color); break;
        case 8: strip.setPixelColor(30, color); break;
        case 9: strip.setPixelColor(29, color); break;
        case 10: strip.setPixelColor(31, color); break;
        //case 11: strip.setPixelColor(, color); break; //not connected in DB9 mode
        case 12: strip.setPixelColor(17, color); break;
        case 13: strip.setPixelColor(18, color); break;
        //case 14: strip.setPixelColor(, color); break; //not connected in DB9 mode
        //case 15: strip.setPixelColor(, color); break; //not connected in DB9 mode
      }
      break;
    case 6: //RJ45
    case 7:
    case 8:
      switch(count){
        case 0: strip.setPixelColor(31, color); break;
        case 1: strip.setPixelColor(27, color); break;
        //case 2: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        //case 3: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        //case 4: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        //case 5: //not connected to mux
        case 6: strip.setPixelColor(28, color); break;
        //case 7: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        case 8: strip.setPixelColor(26, color); break;
        case 9: strip.setPixelColor(30, color); break;
        case 10: strip.setPixelColor(24, color); break;
        //case 11: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        case 12: strip.setPixelColor(25, color); break;
        case 13: strip.setPixelColor(29, color); break;
        //case 14: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        //case 15: strip.setPixelColor(, color); break; //not connected in RJ45 mode
      }
      break;
  }
}
