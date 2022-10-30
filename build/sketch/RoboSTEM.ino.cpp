#include <Arduino.h>
#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
#include "mecanum.h"
/*#include "distances.h"
#include "BNO055.h"
#include "VL53L0X10.h"*/

/*////////////// Declaracion de motores y mecanum ////////////////////
                            Frente
                        ______________
       motor_izq_ade -> |EE         EE| <- motor_der_ade
                        |EE         EE|
                        |             |
                        |             |
                        |EE         EE|
       motor_izq_atr -> |EE         EE| <- motor_der_atr
                        --------------                          */
Motor motor_izq_ade(22, 25, 2, -1, HIGH);
Motor motor_der_ade(26, 30, 4, 1, HIGH);
Motor motor_der_atr(27, 28, 5, 1, HIGH);
Motor motor_izq_atr(23, 24, 3, -1, HIGH);
Mecanum motores(0.2, 0.2, &motor_izq_ade, &motor_der_ade, &motor_der_atr, &motor_izq_atr);

/*////////////// Declaracion de los Sharp ////////////////////
                          Frente
                        ______________
            dis_izq1 -> |  E       E  | <- dis_der1
            dis_freA -> |E           E| <- dis_freB
                        |             |
                        |             |
            dis_atrA -> |E           E| <- dis_atrB
            dis_izq2 -> |  E       E  | <- dis_der2
                        --------------                   */
/*SharpIR dis_freA_sharp( SharpIR::GP2Y0A41SK0F, A0 );
SharpIR dis_freB_sharp( SharpIR::GP2Y0A41SK0F, A1 );
SharpIR dis_der1_sharp( SharpIR::GP2Y0A41SK0F, A2 );
SharpIR dis_der2_sharp( SharpIR::GP2Y0A41SK0F, A3 );
SharpIR dis_atrA_sharp( SharpIR::GP2Y0A41SK0F, A4 );
SharpIR dis_atrB_sharp( SharpIR::GP2Y0A41SK0F, A5 );
SharpIR dis_izq1_sharp( SharpIR::GP2Y0A41SK0F, A6 );
SharpIR dis_izq2_sharp( SharpIR::GP2Y0A41SK0F, A7 );
distancias dis_sensors(&dis_freA_sharp, &dis_freB_sharp, &dis_der1_sharp, &dis_der2_sharp, &dis_atrA_sharp, &dis_atrB_sharp, &dis_izq1_sharp, &dis_izq2_sharp);

//////////////// Declaracion del giroscopio ////////////////////
BNO055 giroscopio;

//////////////// Declaracion de sensores laser ////////////////////
VL53L0X10 dual_laser;*/

#line 48 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
void setup();
#line 54 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
void loop();
#line 48 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
void setup() {
  Serial.begin(115200);
  while (! Serial) { delay(1); }
  /*dual_laser.init(); // Inicializacion de los sensores laser
  giroscopio.init(); // Inicializacion del giroscopio*/
}
void loop() {
  /*dis_sensors.read();                 // Obtiene las mediciones de todos los sensores
  dis_sensors.read_front_sensors();   // Obtiene las distancias de los sensores del centro
  dis_sensors.set_position(180);      // El frente se cambia para atras ( 180° )
  dis_sensors.add_angle2position(90); // Giro virtual de 90°
  int dis1 = dis_sensors.dis_der1;    // Recuperas la distancia del sensor de la derecha 1
  int dis2 = dis_sensors.dis_der2;    // Recuperas la distancia del sensor de la derecha 2

  dual_laser.read();                          // Lectura de los sensores laser
  int dis_laser_der = dual_laser.laser_der;   // Recuperar el valor del sensor de la derecha
  int dis_laser_izq = dual_laser.laser_izq;   // Recuperar el valor del sensor de la izquierda

  giroscopio.read();      // Lectura en x, y, z
  int x = giroscopio.x(); // Recuperar el valor en x del giroscopio
  int y = giroscopio.y(); // Recuperar el valor en y del giroscopio
  int z = giroscopio.z(); // Recuperar el valor en z del giroscopio*/

  motor_der_ade.Move(-255);
  delay(1000);
  motor_der_ade.Move(255);
  delay(1000);

  //motores.Move(180, 0, 255); //(grado, angular, potencia)
}
