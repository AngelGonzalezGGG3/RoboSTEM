#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\TCS34725.h"
#ifndef TCS34725_H
#define TCS34725_H

#include <Arduino.h>
#include <Adafruit_TCS34725.h>
#include "ColorConverterLib.h"

class TCS34725{
  private:
    Adafruit_TCS34725 *sensor_tcs;
    const int interruptPin = 18;
  public:
    TCS34725(Adafruit_TCS34725 *sensor, void (*userFunc)(void));
    void init_beforeLaser();
    void init();
    void read(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
    void get_HSV(int *H, int *S, int *V);
    void get_HSL(int *H, int *S, int *L);
};

#endif
