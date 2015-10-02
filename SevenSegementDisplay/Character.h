#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
  public:
    int number;
    bool dot;
    
    Character(){
      this->number = -1;
      this->dot = false;
    }

    void setNumber(int number){
      this->number = number;
    }

    void setDot(){
      this->dot = true;
    }
};

#endif
