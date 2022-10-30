#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\mecanum.cpp"
#include "mecanum.h"

/*
                                 Frente
                                         ______________
(M1) (DirF1) motor_izquierda_adelante -> |EE         EE| <- motor_derecha_adelante (M2) (DirF2)
                                         |EE         EE|
                                         |             |
                                         |             |
                                         |EE         EE|
   (M1) (DirF4) motor_izquierda_atras -> |EE         EE| <- motor_derecha_atras (M3) (DirF3)
                                         ---------------
*/


Mecanum::Mecanum(double x, double y, Motor *motor_izquierda_adelante, Motor *motor_derecha_adelante, Motor *motor_derecha_atras, Motor *motor_izquierda_atras){
  _x = x;
  _y = y;
  M1 = motor_izquierda_adelante;
  M2 = motor_derecha_adelante;
  M3 = motor_derecha_atras;
  M4 = motor_izquierda_atras;
}

void Mecanum::Move(float deg, float ang, float pot) {
  _deg = deg;
  _ang = ang;
  _pot = pot;

  float DirF1 = 0;
  float DirF2 = 0;
  float DirF3 = 0;
  float DirF4 = 0;

  v[0][0] = _ang;     // W
  v[1][0] = cos(_deg * 0.01745329251); // X
  v[2][0] = sin(_deg * 0.01745329251); // Y

  DirF1 = ((m[0][0] * v[0][0]) + (m[0][1] * v[1][0]) + (m[0][2] * v[2][0])) * _pot;
  DirF2 = ((m[1][0] * v[0][0]) + (m[1][1] * v[1][0]) + (m[1][2] * v[2][0])) * _pot;
  DirF3 = ((m[2][0] * v[0][0]) + (m[2][1] * v[1][0]) + (m[2][2] * v[2][0])) * _pot;
  DirF4 = ((m[3][0] * v[0][0]) + (m[3][1] * v[1][0]) + (m[3][2] * v[2][0])) * _pot;
  Serial.print("M1 |   "); Serial.print(DirF1); Serial.print("     "); Serial.print(DirF2); Serial.println("| M2");
  Serial.print("M4 |   "); Serial.print(DirF4); Serial.print("     "); Serial.print(DirF3); Serial.println("| M3");
  (*M1).Move(DirF1); (*M2).Move(DirF2);
  (*M4).Move(DirF4); (*M3).Move(DirF3);
}
