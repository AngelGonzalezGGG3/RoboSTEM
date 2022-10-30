#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\distances.cpp"
#include "distances.h"

/*
  position_value permite cambiar la posicion del frente de robot, por lo que rotara las valores
  de los sensores Sharp para ajustarlos a la nueva posicion del frente
*/

distancias::distancias(SharpIR *sen_dis_freA, SharpIR *sen_dis_freB, SharpIR *sen_dis_der1, SharpIR *sen_dis_der2, SharpIR *sen_dis_atrA, SharpIR *sen_dis_atrB, SharpIR *sen_dis_izq1, SharpIR *sen_dis_izq2){
  _dis_freA = sen_dis_freA;
  _dis_freB = sen_dis_freB;
  _dis_der1 = sen_dis_der1;
  _dis_der2 = sen_dis_der2;
  _dis_atrA = sen_dis_atrA;
  _dis_atrB = sen_dis_atrB;
  _dis_izq1 = sen_dis_izq1;
  _dis_izq2 = sen_dis_izq2;
}

void distancias::set_position(int position_value){
  position = position_value;
}

void distancias::add_angle2position(int angle){
  position = position + angle;

  if(position >= 360) position = position-360;
  if(position < 0)    position = 360 + position;
}

void distancias::read(){
  if(position == 0){
    dis_freA = (*_dis_freA).getDistance();  //                          FRENTE
    dis_freB = (*_dis_freB).getDistance();  //                      ______________ 
    dis_der1 = (*_dis_der1).getDistance();  //   (izq1) dis_izq1 -> |  E       E  | <- dis_der1 (der1)
    dis_der2 = (*_dis_der2).getDistance();  //   (freA) dis_freA -> |E           E| <- dis_freB (freB)
    dis_atrA = (*_dis_atrA).getDistance();  //                      |             |
    dis_atrB = (*_dis_atrB).getDistance();  //                      |             |
    dis_izq1 = (*_dis_izq1).getDistance();  //   (atrA) dis_atrA -> |E           E| <- dis_atrB (atrB)
    dis_izq2 = (*_dis_izq2).getDistance();  //   (izq2) dis_izq2 -> |  E       E  | <- dis_der2 (der2)
  }                                         //                      --------------
  
  else if(position == 90){
    dis_freA = (*_dis_der1).getDistance();  //                      ______________
    dis_freB = (*_dis_der2).getDistance();  //   (atrA) dis_izq1 -> |  E       E  | <- dis_der1 (freA)
    dis_der1 = (*_dis_atrB).getDistance();  //   (izq2) dis_freA -> |E           E| <- dis_freB (izq1)
    dis_der2 = (*_dis_atrA).getDistance();  //                      |             |   FRENTE
    dis_atrA = (*_dis_izq1).getDistance();  //                      |             |
    dis_atrB = (*_dis_izq2).getDistance();  //   (der2) dis_atrA -> |E           E| <- dis_atrB (der1)
    dis_izq1 = (*_dis_freB).getDistance();  //   (atrB) dis_izq2 -> |  E       E  | <- dis_der2 (freB)
    dis_izq2 = (*_dis_freA).getDistance();  //                      --------------
  }
  
  else if(position == 180){
    dis_freA = (*_dis_atrB).getDistance();  //                      ______________
    dis_freB = (*_dis_atrA).getDistance();  //   (der2) dis_izq1 -> |  E       E  | <- dis_der1 (izq2)
    dis_der1 = (*_dis_izq2).getDistance();  //   (atrB) dis_freA -> |E           E| <- dis_freB (atrA)
    dis_der2 = (*_dis_izq1).getDistance();  //                      |             | 
    dis_atrA = (*_dis_freB).getDistance();  //                      |             |
    dis_atrB = (*_dis_freA).getDistance();  //   (freB) dis_atrA -> |E           E| <- dis_atrB (freA)
    dis_izq1 = (*_dis_der2).getDistance();  //   (der1) dis_izq2 -> |  E       E  | <- dis_der2 (izq1)
    dis_izq2 = (*_dis_der1).getDistance();  //                      --------------
  }                                         //                          FRENTE
  
  else{
    dis_freA = (*_dis_izq1).getDistance();  //                      ______________
    dis_freB = (*_dis_izq2).getDistance();  //   (freB) dis_izq1 -> |  E       E  | <- dis_der1 (atrB)
    dis_der1 = (*_dis_freA).getDistance();  //   (der1) dis_freA -> |E           E| <- dis_freB (der2)
    dis_der2 = (*_dis_freB).getDistance();  //             FRENTE   |             | 
    dis_atrA = (*_dis_der2).getDistance();  //                      |             |
    dis_atrB = (*_dis_der1).getDistance();  //   (izq1) dis_atrA -> |E           E| <- dis_atrB (izq2)
    dis_izq1 = (*_dis_atrA).getDistance();  //   (freA) dis_izq2 -> |  E       E  | <- dis_der2 (atrA)
    dis_izq2 = (*_dis_atrB).getDistance();  //                      --------------
  }
}

void distancias::read_front_sensors(){
  if(position == 0){
    dis_freA = (*_dis_freA).getDistance();
    dis_freB = (*_dis_freB).getDistance();
  }
  else if(position == 90){
    dis_freA = (*_dis_der1).getDistance();
    dis_freB = (*_dis_der2).getDistance();
  }
  else if(position == 180){
    dis_freA = (*_dis_atrB).getDistance();
    dis_freB = (*_dis_atrA).getDistance();
  }
  else if(position == 270){
    dis_freA = (*_dis_izq1).getDistance();
    dis_freB = (*_dis_izq2).getDistance();
  }
}