#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\mecanum.h"
#ifndef MECANUM_H
#define MECANUM_H

#include <Arduino.h>
#include "motor.h"

class Mecanum {
  private:
    float _x       = 0;
    float _y       = 0;
    float _deg   = 0;
    float _ang  = 0;
    float _pot = 0;

    Motor *M1;
    Motor *M2;
    Motor *M3;
    Motor *M4;

    float m[4][3] = {
      { -_x - _y, 1, -1},
      { _x + _y, 1, 1},
      { _x + _y,  1, -1},
      { -_x - _y, 1, 1}
    };

    double v[3][1] = {
      {0},
      {0},
      {0}
    };

  public:
    Mecanum(double x, double y, Motor *motor_izquierda_adelante, Motor *motor_derecha_adelante, Motor *motor_derecha_atras, Motor *motor_izquierda_atras);
    void Move(float deg, float ang, float pot);
    void turnLeft(float pot);
    void turnRight(float pot);
};

#endif
