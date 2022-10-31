#include "TCS34725.h"

TCS34725::TCS34725(Adafruit_TCS34725 *sensor, void (*userFunc)(void)){
  sensor_tcs = sensor;
  pinMode(interruptPin, INPUT_PULLUP); //TCS interrupt output is Active-LOW and Open-Drain
  attachInterrupt(digitalPinToInterrupt(interruptPin), *userFunc, FALLING);
}
void TCS34725::read(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
  *c = (*sensor_tcs).read16(TCS34725_CDATAL);
  *r = (*sensor_tcs).read16(TCS34725_RDATAL);
  *g = (*sensor_tcs).read16(TCS34725_GDATAL);
  *b = (*sensor_tcs).read16(TCS34725_BDATAL);
  (*sensor_tcs).clearInterrupt();
}
void TCS34725::init_beforeLaser(){
  pinMode(45,OUTPUT);
  digitalWrite(45,LOW);
}
void TCS34725::init(){
  digitalWrite(45,HIGH);
  delay(10);

  if ((*sensor_tcs).begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  (*sensor_tcs).write8(TCS34725_PERS, TCS34725_PERS_NONE); 
  (*sensor_tcs).setInterrupt(true);
  Serial.flush();
}
