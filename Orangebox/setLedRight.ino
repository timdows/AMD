void setLedByIntRight(int count, uint32_t color){
  switch(mode){
    case 0: //DB15
    case 3:
    case 6:
      switch(count){
        case 0: strip.setPixelColor(14, color); break;
        case 1: strip.setPixelColor(11, color); break;
        case 2: strip.setPixelColor(13, color); break;
        case 3: strip.setPixelColor(12, color); break;
        case 4: strip.setPixelColor(0, color); break;
        case 5: strip.setPixelColor(3, color); break;
        case 6: strip.setPixelColor(1, color); break;
        case 7: strip.setPixelColor(2, color); break;
        case 8: strip.setPixelColor(5, color); break;
        case 9: strip.setPixelColor(6, color); break;
        case 10: strip.setPixelColor(4, color); break;
        case 11: strip.setPixelColor(7, color); break;
        case 12: strip.setPixelColor(9, color); break;
        //case 13: //not connected to mux
        case 14: strip.setPixelColor(10, color); break;
        case 15: strip.setPixelColor(8, color); break;
      }
      break;
    case 1: //DB9
    case 4:
    case 7:
      switch(count){
        //case 0: strip.setPixelColor(, color); break; //not connected in DB9 mode
        case 1: strip.setPixelColor(5, color); break;
        case 2: strip.setPixelColor(3, color); break;
        case 3: strip.setPixelColor(4, color); break;
        //case 4: strip.setPixelColor(, color); break; //not connected in DB9 mode
        case 5: strip.setPixelColor(10, color); break;
        //case 6: strip.setPixelColor(, color); break; //not connected in DB9 mode
        case 7: strip.setPixelColor(11, color); break;
        case 8: strip.setPixelColor(8, color); break;
        //case 9: strip.setPixelColor(, color); break; //not connected in DB9 mode
        case 10: strip.setPixelColor(9, color); break;
        //case 11: strip.setPixelColor(, color); break; //not connected in DB9 mode
        case 12: strip.setPixelColor(7, color); break;
        //case 13: //not connected to mux
        case 14: strip.setPixelColor(6, color); break;
        //case 15: strip.setPixelColor(, color); break; //not connected in DB9 mode
      }
      break;
    case 2: //RJ45
    case 5:
    case 8:
      switch(count){
        //case 0: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        case 1: strip.setPixelColor(6, color); break;
        case 2: strip.setPixelColor(0, color); break;
        case 3: strip.setPixelColor(2, color); break;
        //case 4: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        case 5: strip.setPixelColor(5, color); break;
        //case 6: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        case 7: strip.setPixelColor(1, color); break;
        case 8: strip.setPixelColor(3, color); break;
        //case 9: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        case 10: strip.setPixelColor(7, color); break;
        //case 11: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        //case 12: strip.setPixelColor(, color); break; //not connected in RJ45 mode
        //case 13: //not connected to mux
        case 14: strip.setPixelColor(4, color); break;
        //case 15: strip.setPixelColor(, color); break; //not connected in RJ45 mode
      }
      break;
  }
}
