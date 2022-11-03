#include "mecanum.h"
#include "TCS34725.h"
#include "VL53L0X10.h"
#include "BNO055.h"
#include "distances.h"
#include "HCSR04.h"
#include "Servo.h"

#define Scan_I2C_devices false

#if Scan_I2C_devices
  void I2C_Scanner();

#else
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
  volatile boolean RGB_ready = false;   // Variable que indica que se puede leer los nuevos datos del RGB
  int color_anterior = 0;               // Variable donde se almacenara el color anterior
  void isr() {RGB_ready = true;}        // Funcion de interrupcion
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_240MS, TCS34725_GAIN_1X);
  TCS34725 RGB(&tcs, isr);

  //////////////// Declaracion de los actuadores de la pinza ////////////////////
  Motor MotorMini(6,7,1,LOW);
  void bajar_pinza();
  void subir_pinza();

  //////////////// Declaracion del Ultrasonico ////////////////////
  HCSR04 ultrasonico(46, 47);

  //////////////// Declaracion del Servomotor ////////////////////
  Servo servo;

  int contador_paredes;  //Contador para comprobar que no esta pegado a las paredes
  
#endif

void setup() {
  Serial.begin(115200);           // Inicializacion del Serial
  while (! Serial) { delay(1); }  // Esperar la Inicializacion del serial
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);
  delay(2000);

  #if Scan_I2C_devices
    Wire.begin();
  #else
    
    RGB.init_beforeLaser();         // Inicializacion del RGB antes de los laser
    delay(100);
    giroscopio.init();              // Inicializacion del giroscopio
    delay(100);
    dual_laser.init();              // Inicializacion de los sensores laser
    delay(100);
    RGB.init();                     // Inicializacion del RGB
    servo_init();
  #endif
}

void loop() {

  #if Scan_I2C_devices
    I2C_Scanner();
  #else
  
    giroscopio.read();      // Lectura en x, y, z
    int z = giroscopio.z(); // Recuperar el valor en z del giroscopio
    
    if(z > 2 && z <= 5)          motores.Move(dis_sensors.position, 2,255); // Si el desfase es pequeño en positivo
    else if(z >= 355 && z < 358) motores.Move(dis_sensors.position,-2,255); // Si el desfase es pequeño en negativo
    else if(z > 5 && z <= 180)   motores.turnLeft(100);                     // Si el desfase es grande en positivo
    else if(z > 180 && z < 355)  motores.turnRight(100);                    // Si el desfase es grande en negativo
    else                         motores.Move(dis_sensors.position,0,255);  // Si no hay desfase avanza

    dis_sensors.read_front_sensors();                               // Lee los sensores de distancia del frente
    if((dis_sensors.dis_freA+dis_sensors.dis_freB)/2 <= 10){        // Si los sensores del frente son menor a 10cm
      //FIXME: Comprobar antes de cambiar la direccion que este algunos cm separado el robot de la pared de al frente
      dis_sensors.read_right_sensors();                             // Se lee los sensores de la derecha
      dis_sensors.read_left_sensors();                              // Se lee los sensores de la derecha

      int prom_fre = (dis_sensors.dis_freA+dis_sensors.dis_freB)/2; // Se promedian los sensores de al frente
      if(prom_fre <= 6){                                            // Si el robot esta muy pegado a la pared de al frente
        motores.Move(dis_sensors.position_back,0,255);              // Se mueve hacia atras
        while(prom_fre > 6){                                        // Hasta que la distancia sea mayor a 6cm
          dis_sensors.read_front_sensors();
          prom_fre = (dis_sensors.dis_freA+dis_sensors.dis_freB)/2;
        }
      }

      int prom_der = (dis_sensors.dis_der1+dis_sensors.dis_der2)/2; // Se promedian los sensores de la derecha
      int prom_izq = (dis_sensors.dis_izq1+dis_sensors.dis_izq2)/2; // Se promedian los sensores de la izquierda
      int diff = prom_der-prom_izq;                                 // Se obtiene la diferencia de los dos sensores

      if(diff > 0)      dis_sensors.add_angle2position(90);         // Si hay salida a la derecha
      else if(diff < 0) dis_sensors.add_angle2position(-90);        // Si hay salida a la izquierda
    }
    
    if (RGB_ready) {                                        // Si el RGB tiene un nuevo valor
      int color = encontrar_color();                        // Obtener el color (Blanco(0), Rojo(1), Verde(2) o Azul(3))
      if(color != color_anterior){                          // Si el color nuevo es diferente al color anterior
        if(color == 1){                                     // Si el color es rojo
          dis_sensors.read_right_sensors();                 // Se lee los sensores de la derecha
          if(dis_sensors.dis_der2 < 10){                    // Si el sensor de la derecha-atras detecta pared
            while (dis_sensors.dis_der2 < 10) {             // Avanza hasta que deje de detectar pared
              motores.Move(dis_sensors.position, 0, 255);   
              dis_sensors.read_right_sensors();
            }
            motores.stop();
          }
          else if(dis_sensors.dis_der1 < 10){                   // Si el sensor de la derecha-adelante detecta pared
            while (dis_sensors.dis_der1 < 10) {                 // Retrocede hasta que deje de detectar pared
              motores.Move(dis_sensors.position_back, 0, 255);
              dis_sensors.read_right_sensors();
            }
            motores.stop();
          }
          dis_sensors.add_angle2position(90);
        }
        if(color == 3){
          dis_sensors.read_left_sensors();                 // Se lee los sensores de la derecha
          if(dis_sensors.dis_izq2 < 10){                    // Si el sensor de la derecha-atras detecta pared
            while (dis_sensors.dis_izq2 < 10) {             // Avanza hasta que deje de detectar pared
              motores.Move(dis_sensors.position, 0, 255);   
              dis_sensors.read_left_sensors();
            }
            motores.stop();
          }
          else if(dis_sensors.dis_izq1 < 10){                   // Si el sensor de la derecha-adelante detecta pared
            while (dis_sensors.dis_izq1 < 10) {                 // Retrocede hasta que deje de detectar pared
              motores.Move(dis_sensors.position_back, 0, 255);
              dis_sensors.read_left_sensors();
            }
            motores.stop();
          }
          dis_sensors.add_angle2position(-90); // Si el color es azul
        }
        color_anterior = color;                             // Se actualiza el color anterior con el color nuevo
      }
      RGB_ready = false;                                    // Reiniciamos la variable para obtener un nuevo valor del RGB
    }

    //FIXME: Comprobar que no este tan pegado a las paredes de la derecha o la izquierda, que sea en intervalos de tiempo
    if(contador_paredes >= 10){                                     // Cada 10 iteraciones del codigo comprueba si no se ha pegado a alguna pared
      dis_sensors.read_right_sensors();                             // Se lee los sensores de la derecha
      dis_sensors.read_left_sensors();                              // Se lee los sensores de la izquierda

      int prom_der = (dis_sensors.dis_der1+dis_sensors.dis_der2)/2; // Se promedian los sensores de la derecha
      int prom_izq = (dis_sensors.dis_izq1+dis_sensors.dis_izq2)/2; // Se promedian los sensores de la izquierda
      
      //
      if(prom_der <= 6){                                            // Si los sensores de la derecha detectan que esta cerca de la pared
        motores.Move(dis_sensors.position_left, 0, 255);            // Se aleja de la pared
        while (prom_der > 6 ){                                      // Hasta que detecte un distancia sea mayor a 6cm
          dis_sensors.read_right_sensors();
          prom_der = (dis_sensors.dis_der1+dis_sensors.dis_der2)/2; 
        }
      }
      else if(prom_izq <= 6){                                       // Si los sensores de la izquierda detectan que esta cerca de la pared
        motores.Move(dis_sensors.position_right, 0, 255);           // Se aleja de la pared
        while (prom_izq > 6 ){                                      // Hasta que detecte un distancia sea mayor a 6cm
          dis_sensors.read_left_sensors();
          prom_izq = (dis_sensors.dis_izq1+dis_sensors.dis_izq2)/2; 
        }
      }
      contador_paredes = 0;                                         // Reinicia la variable del contador
    }
    else contador_paredes++;                                        // Si aun no llega al valor se realiza un conteo
    
  #endif
}

#if Scan_I2C_devices
  void I2C_Scanner(){
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for(address = 1; address < 127; address++ ) 
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if (error == 0)
      {
        Serial.print("I2C device found at address 0x");
        if (address<16) 
          Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("  !");

        nDevices++;
      }
      else if (error==4) 
      {
        Serial.print("Unknown error at address 0x");
        if (address<16) 
          Serial.print("0");
        Serial.println(address,HEX);
      }    
    }
    if (nDevices == 0)
      Serial.println("No I2C devices found\n");
    else
      Serial.println("done\n");

    //delay(1000);           // wait 5 seconds for next scan
  }
#else


  int encontrar_color(){
    int H, S, V;
      RGB.get_HSV(&H,&S,&V);

      Serial.print("H = "); Serial.print(H);

      if(H < 14) return 1; //Serial.print("Rojo");
      else if(H < 48) return 0; //Serial.print("Blanco");
      else if(H < 125) return 2; //Serial.print("Verde");
      else return 3; //Serial.print("Azul");
  }

  void servoMove(int ang_inicial, int ang_final, int time){
    int diff_ang = ang_final - ang_inicial;
    if(diff_ang > 0){
      for(int i = ang_inicial; i <= ang_final ; i++ ){
        servo.write(i);
        Serial.println(i);
        delay(time);
      }
    }
    else{
      for(int i = ang_inicial; i >= ang_final ; i-- ){
        servo.write(i);
        Serial.println(i);
        delay(time);
      }
    }
  }
  void servo_init(){
    servo.attach(11);
    MotorMini.Move(-180);
    delay(1000);
    MotorMini.Move(0);
    servo.write(150);
  }

  void bajar_pinza(){
    servoMove(150, 65, 30);
    MotorMini.Move(150);
    delay(500);
    /*MotorMini.Move(-10);
    delay(100);*/
    MotorMini.Move(0);
  }
  void subir_pinza(){
    MotorMini.Move(-180);
    delay(3000);
    MotorMini.Move(-50);
    servoMove(65, 150, 30);
  }
#endif