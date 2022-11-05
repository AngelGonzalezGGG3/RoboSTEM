#include "BNO055.h"

void BNO055::init(){
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);
  delay(100);
  if (!_bno.begin()) { Serial.print("No BNO055 detected"); while (1); }
}

void BNO055::read(){ 
  _bno.getEvent(&_orientationData, Adafruit_BNO055::VECTOR_EULER);
}

float BNO055::z(){
  int _z = _orientationData.orientation.x - z_offset;
  if(_z < 0) _z = 360 + _z;
  Serial.println(_z);
  return _z;
}

float BNO055::y(){
  return _orientationData.orientation.y;
}

float BNO055::x(){
  return _orientationData.orientation.z;
}

void BNO055::set_z_offset(int degree){
  z_offset = z_offset + degree;
  
  if(z_offset >= 360) z_offset = z_offset - 360;
  else if(z_offset < 0) z_offset = 360 + z_offset;
}