#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\VL53L0X10.h"
#ifndef VL53L0X10_H
#define VL53L0X10_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_VL53L0X.h>

  class VL53L0X10{
    private:
      VL53L0X_RangingMeasurementData_t measure1; Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
      VL53L0X_RangingMeasurementData_t measure2; Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
      void setID();

    public:
      uint16_t laser_der = 0;
      uint16_t laser_izq = 0;

      void init();
      void read();
  };

#endif
