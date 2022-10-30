#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
  private:
    int _dir;
    int _pin1;
    int _pin2;
    int _pwm;
    bool _type_Stop;
  public:
    Motor(int pin1, int pin2, int pwm, int dir, bool type_Stop);
    void Move(float vel);
};

#endif
