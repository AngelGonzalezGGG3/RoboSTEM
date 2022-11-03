#include "HCSR04.h"

HCSR04::HCSR04(int trigger, int echo){
  _trigger = trigger;
  _echo = echo;

  pinMode(_trigger, OUTPUT);
  pinMode(_echo, INPUT);
  digitalWrite(_trigger, LOW);
}

int HCSR04::read(){
  digitalWrite(_trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(_trigger, LOW);
  
  long t = pulseIn(_echo, HIGH); //obtenemos el ancho del pulso
  return t/59;
}