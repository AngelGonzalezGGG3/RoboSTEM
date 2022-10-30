#ifndef DISTANCES_H
#define DISTANCES_H

#include <Arduino.h>
#include <SharpIR.h>

  class distancias{
    private:
      SharpIR *_dis_freA;
      SharpIR *_dis_freB;
      SharpIR *_dis_der1;
      SharpIR *_dis_der2;
      SharpIR *_dis_atrA;
      SharpIR *_dis_atrB;
      SharpIR *_dis_izq1;
      SharpIR *_dis_izq2;
    
    public:
      int dis_freA = 0;
      int dis_freB = 0;
      int dis_der1 = 0;
      int dis_der2 = 0;
      int dis_atrA = 0;
      int dis_atrB = 0;
      int dis_izq1 = 0;
      int dis_izq2 = 0;
      int position= 0;

      distancias(SharpIR *sen_dis_freA,
                 SharpIR *sen_dis_freB,
                 SharpIR *sen_dis_der1,
                 SharpIR *sen_dis_der2,
                 SharpIR *sen_dis_atrA,
                 SharpIR *sen_dis_atrB,
                 SharpIR *sen_dis_izq1,
                 SharpIR *sen_dis_izq2);
      void set_position(int position);
      void add_angle2position(int angle);
      void read();
      void read_front_sensors();
  };

#endif
