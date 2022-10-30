#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\BNO055.h"
#ifndef BNO055_H
#define BNO055_H

#include <Arduino.h>

#include <Adafruit_BNO055.h>

  class BNO055{
    private: 
      Adafruit_BNO055 _bno = Adafruit_BNO055(55, 0x29);
      sensors_event_t _orientationData; 

    public:
      void init();
      void read();
      float x();
      float y();
      float z();
  };

#endif
