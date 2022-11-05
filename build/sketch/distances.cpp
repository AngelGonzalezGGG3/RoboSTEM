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

  set_position(0);
}

void distancias::set_position(int position_value){
  position = position_value;
  set_postion_back();
  set_postion_left();
  set_postion_right();
}

void distancias::set_postion_back(){
  position_back = position + 180;

  if(position_back >= 360)   position_back = position_back-360;
  else if(position_back < 0) position_back = 360 + position_back;
}

void distancias::set_postion_left(){
  position_left = position - 90;

  if(position_left >= 360)   position_left = position_left-360;
  else if(position_left < 0) position_left = 360 + position_left;
}

void distancias::set_postion_right(){
  position_right = position + 90;

  if(position_right >= 360)   position_right = position_right-360;
  else if(position_right < 0) position_right = 360 + position_right;
}

void distancias::add_angle2position(int angle){
  position = position + angle;

  if(position >= 360)   position = position-360;
  else if(position < 0) position = 360 + position;

  set_postion_back();
  set_postion_left();
  set_postion_right();
}

void distancias::read(){
  read_back_sensors();
  read_front_sensors();
  read_left_sensors();
  read_right_sensors();
}

void distancias::read_front_sensors(){
  
  dis_freA = 0;
  dis_freB = 0;

  for(int i = 0; i < promedio ; i++){
    if(position == 0){
      dis_freA += (*_dis_freA).getDistance();
      dis_freB += (*_dis_freB).getDistance();
    }
    else if(position == 90){
      dis_freA += (*_dis_der1).getDistance();
      dis_freB += (*_dis_der2).getDistance();
    }
    else if(position == 180){
      dis_freA += (*_dis_atrB).getDistance();
      dis_freB += (*_dis_atrA).getDistance();
    }
    else if(position == 270){
      dis_freA += (*_dis_izq1).getDistance();
      dis_freB += (*_dis_izq2).getDistance();
    }
  }

  dis_freA = dis_freA/promedio;
  dis_freB = dis_freB/promedio;
}

void distancias::read_right_sensors(){
  
  dis_der1 = 0;
  dis_der2 = 0;

  for(int i = 0; i < promedio ; i++){
    if(position == 0){
      dis_der1 += (*_dis_der1).getDistance();
      dis_der2 += (*_dis_der2).getDistance();
    }
    else if(position == 90){
      dis_der1 += (*_dis_atrB).getDistance();
      dis_der2 += (*_dis_atrA).getDistance();
    }
    else if(position == 180){
      dis_der1 += (*_dis_izq2).getDistance();
      dis_der2 += (*_dis_izq1).getDistance();
    }
    else if(position == 270){
      dis_der1 += (*_dis_freA).getDistance();
      dis_der2 += (*_dis_freB).getDistance();
    }
  }

  dis_der1 = dis_der1/promedio;
  dis_der2 = dis_der2/promedio;
}

void distancias::read_back_sensors(){
  
  dis_atrA = 0;
  dis_atrB = 0;

  for(int i = 0; i < promedio ; i++){
    if(position == 0){
      dis_atrA += (*_dis_atrA).getDistance();
      dis_atrB += (*_dis_atrB).getDistance();
    }
    else if(position == 90){
      dis_atrA += (*_dis_izq1).getDistance();
      dis_atrB += (*_dis_izq2).getDistance();
    }
    else if(position == 180){
      dis_atrA += (*_dis_freB).getDistance();
      dis_atrB += (*_dis_freA).getDistance();
    }
    else if(position == 270){
      dis_atrA += (*_dis_der2).getDistance();
      dis_atrB += (*_dis_der1).getDistance();
    }
  }

  dis_atrA = dis_atrA/promedio;
  dis_atrB = dis_atrB/promedio;
}

void distancias::read_left_sensors(){
  
  dis_izq1 = 0;
  dis_izq2 = 0;

  for(int i = 0; i < promedio ; i++){
    if(position == 0){
      dis_izq1 += (*_dis_izq1).getDistance();  //   (atrA) dis_atrA -> |E           E| <- dis_atrB (atrB)
      dis_izq2 += (*_dis_izq2).getDistance();
    }
    else if(position == 90){
      dis_izq1 += (*_dis_freB).getDistance();  //   (atrB) dis_izq2 -> |  E       E  | <- dis_der2 (freB)
      dis_izq2 += (*_dis_freA).getDistance();
    }
    else if(position == 180){
      dis_izq1 += (*_dis_der2).getDistance();  //   (der1) dis_izq2 -> |  E       E  | <- dis_der2 (izq1)
      dis_izq2 += (*_dis_der1).getDistance();
    }
    else if(position == 270){
      dis_izq1 += (*_dis_atrA).getDistance();  //   (freA) dis_izq2 -> |  E       E  | <- dis_der2 (atrA)
      dis_izq2 += (*_dis_atrB).getDistance();
    }
  }

  dis_izq1 = dis_izq1/promedio;
  dis_izq2 = dis_izq2/promedio;
}