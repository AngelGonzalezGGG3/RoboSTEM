#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\TCS34725.cpp"
#include "TCS34725.h"

TCS34725::TCS34725(Adafruit_TCS34725 *sensor, void (*userFunc)(void)){
  sensor_tcs = sensor;
  pinMode(interruptPin, INPUT_PULLUP); //TCS interrupt output is Active-LOW and Open-Drain
  attachInterrupt(digitalPinToInterrupt(interruptPin), *userFunc, FALLING);
}
void TCS34725::read(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *clear) {
  *clear = (*sensor_tcs).read16(TCS34725_CDATAL);
  *red = (*sensor_tcs).read16(TCS34725_RDATAL);
  *green = (*sensor_tcs).read16(TCS34725_GDATAL);
  *blue = (*sensor_tcs).read16(TCS34725_BDATAL);
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

void TCS34725::get_HSV(int *H, int *S, int *V){
    uint16_t red, green, blue, clear;
    read(&red, &green, &blue, &clear);

    uint32_t sum = clear;
    float r, g, b;
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;

    r *= 256; g *= 256; b *= 256;

    double hue, saturation, value;
    ColorConverter::RgbToHsv(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), hue, saturation, value);

    *H = hue*360;
    *S = saturation*100;
    *V = value*100;
}

void TCS34725::get_HSL(int *H, int *S, int *L){
    uint16_t red, green, blue, clear;
    read(&red, &green, &blue, &clear);

    uint32_t sum = clear;
    float r, g, b;
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;

    r *= 256; g *= 256; b *= 256;

    double hue, saturation, lighting;
    ColorConverter::RgbToHsl(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), hue, saturation, lighting);

    *H = hue*360;
    *S = saturation*100;
    *L = lighting*100;
}