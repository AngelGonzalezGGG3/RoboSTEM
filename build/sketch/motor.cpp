#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\motor.cpp"
#include "Motor.h"

/*
  type_Stop se refiere si el puente H soporta el apagado con HIGH/HIGH o con LOW/LOW
  en caso de ser HIGH/HIGH se utiliza el parametro HIGH, en caso de ser LOW/LOW, se
  el parametro LOW

  dir puede tomar 1 o -1 en caso de poner -1 invertira la polaridad del motor
*/

Motor::Motor(int pin1, int pin2, int pwm, int dir, bool type_Stop) {
  _pin1 = pin1;
  _pin2 = pin2;
  _pwm  = pwm;
  _type_Stop = type_Stop;

  (dir >= 0) ? _dir = 1 : _dir = -1;

  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  pinMode(_pwm, OUTPUT);
}

void Motor::Move(float vel) {

  vel = vel * _dir;

  if (vel > 0) {
    if (vel > 255) vel = 255;
    digitalWrite(_pin1,HIGH);
    digitalWrite(_pin2, LOW);
    analogWrite(_pwm,   vel);
  }
  else if (vel < 0) {
    if (vel < -255) vel = -255;
    digitalWrite(_pin1,LOW);
    digitalWrite(_pin2,HIGH);
    analogWrite(_pwm,  -vel);
  }
  else {
    digitalWrite(_pin1,_type_Stop);
    digitalWrite(_pin2,_type_Stop);
  }
  //Serial.print(vel); Serial.print("    ");
}
