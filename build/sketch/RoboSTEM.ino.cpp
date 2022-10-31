#include <Arduino.h>
#line 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
#include "mecanum.h"
#include "TCS34725.h"
#include "VL53L0X10.h"
#include "BNO055.h"
#include "distances.h"

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
Motor motor_der_ade(26, 29, 4, -1, HIGH);
Motor motor_der_atr(27, 28, 5, -1, HIGH);
Motor motor_izq_atr(23, 24, 3, -1, HIGH);
Mecanum motores(0.2, 0.2,&motor_izq_ade,&motor_der_ade, &motor_der_atr, &motor_izq_atr);

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
SharpIR dis_freA_sharp( SharpIR::GP2Y0A41SK0F, A2 ); //
SharpIR dis_freB_sharp( SharpIR::GP2Y0A41SK0F, A6 ); //
SharpIR dis_der1_sharp( SharpIR::GP2Y0A41SK0F, A1 ); //
SharpIR dis_der2_sharp( SharpIR::GP2Y0A41SK0F, A7 ); //
SharpIR dis_atrA_sharp( SharpIR::GP2Y0A41SK0F, A3 ); //
SharpIR dis_atrB_sharp( SharpIR::GP2Y0A41SK0F, A5 ); //
SharpIR dis_izq1_sharp( SharpIR::GP2Y0A41SK0F, A0 );
SharpIR dis_izq2_sharp( SharpIR::GP2Y0A41SK0F, A4 ); //
distancias dis_sensors(&dis_freA_sharp, &dis_freB_sharp, &dis_der1_sharp, &dis_der2_sharp, &dis_atrA_sharp, &dis_atrB_sharp, &dis_izq1_sharp, &dis_izq2_sharp);

//////////////// Declaracion del giroscopio ////////////////////
BNO055 giroscopio;

//////////////// Declaracion de sensores laser ////////////////////
VL53L0X10 dual_laser;

//////////////// Declaracion del sensor de color ////////////////////
volatile boolean state = false;   // Variable que indica que se puede leer los nuevos datos del RGB
#line 51 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
void isr();
#line 58 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
void setup();
#line 68 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
void loop();
#line 51 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
void isr() {state = true;}        // Funcion de interrupcion
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
TCS34725 RGB(&tcs, isr);

//////////////// Declaracion de los actuadores de la pinza ////////////////////
Motor MotorMini(6,7,1,LOW);

void setup() {
  Serial.begin(115200);           // Inicializacion del Serial
  while (! Serial) { delay(1); }  // Esperar la Inicializacion del serial

  RGB.init_beforeLaser();         // Inicializacion del RGB antes de los laser
  dual_laser.init();              // Inicializacion de los sensores laser
  giroscopio.init();              // Inicializacion del giroscopio
  RGB.init();                     // inicializacion del RGB
}

void loop() {
  /*dis_sensors.read_front_sensors();   // Obtiene las distancias de los sensores del centro
  dis_sensors.set_position(180);      // El frente se cambia para atras ( 180° )
  dis_sensors.add_angle2position(90); // Giro virtual de 90°*/
  dis_sensors.read();                 // Obtiene las mediciones de todos los sensores
  Serial.print("  der1 = ");Serial.print(dis_sensors.dis_der1);    // Recuperas la distancia del sensor de la derecha 1
  Serial.print("  der2 = ");Serial.print(dis_sensors.dis_der2);    // Recuperas la distancia del sensor de la derecha 2
  Serial.print("  izq1 = ");Serial.print(dis_sensors.dis_izq1);    // Recuperas la distancia del sensor de la derecha 2
  Serial.print("  izq2 = ");Serial.print(dis_sensors.dis_izq2);    // Recuperas la distancia del sensor de la derecha 2
  Serial.print("  freA = ");Serial.print(dis_sensors.dis_freA);    // Recuperas la distancia del sensor de la derecha 2
  Serial.print("  freB = ");Serial.print(dis_sensors.dis_freB);    // Recuperas la distancia del sensor de la derecha 2
  Serial.print("  atrA = ");Serial.print(dis_sensors.dis_atrA);    // Recuperas la distancia del sensor de la derecha 2
  Serial.print("  atrB = ");Serial.println(dis_sensors.dis_atrB);    // Recuperas la distancia del sensor de la derecha 2

  /*dual_laser.read();                          // Lectura de los sensores laser
  int dis_laser_der = dual_laser.laser_der;   // Recuperar el valor del sensor de la derecha
  int dis_laser_izq = dual_laser.laser_izq;   // Recuperar el valor del sensor de la izquierda
  //Serial.print(dis_laser_der); Serial.print(""); Serial.print(dis_laser_izq); 

  giroscopio.read();      // Lectura en x, y, z
  int x = giroscopio.x(); // Recuperar el valor en x del giroscopio
  int y = giroscopio.y(); // Recuperar el valor en y del giroscopio
  int z = giroscopio.z(); // Recuperar el valor en z del giroscopio
  Serial.print("x = "); Serial.print(x);
  Serial.print("y = "); Serial.print(y);
  Serial.print("z = "); Serial.println(z);

  if (state) {
    uint16_t r, g, b, c;
    RGB.read(&r, &g, &b, &c);
    
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");
    Serial.flush();

    state = false;
  }*/
}
