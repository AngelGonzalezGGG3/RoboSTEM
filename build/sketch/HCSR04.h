#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\HCSR04.h"
#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

class HCSR04{
  private:
    int _trigger;
    int _echo;
  public:
    HCSR04(int trigger, int echo);
    int read();
};

#endif
