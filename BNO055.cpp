#include "BNO055.h"

void BNO055::init(){
  if (!_bno.begin()) { Serial.print("No BNO055 detected"); while (1); }
}

void BNO055::read(){ 
  _bno.getEvent(&_orientationData, Adafruit_BNO055::VECTOR_EULER);
}

float BNO055::z(){
  return _orientationData.orientation.x;
}

float BNO055::y(){
  return _orientationData.orientation.y;
}

float BNO055::x(){
  return _orientationData.orientation.z;
}