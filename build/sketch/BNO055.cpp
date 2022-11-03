#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\BNO055.cpp"
#include "BNO055.h"

void BNO055::init(){
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);
  delay(100);
  if (!_bno.begin()) { Serial.print("No BNO055 detected"); while (1); }

  /*while(1){
    Serial.print("while");
    if(_bno.begin()){
      break;
      Serial.println("BNO055 detected");
    }
    else{
      Serial.println("No BNO055 detected");
      digitalWrite(43, LOW);
      delay(1000);
      digitalWrite(43, HIGH);
      delay(100);
    }
  }*/
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