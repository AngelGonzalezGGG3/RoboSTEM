# 1 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
# 2 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino" 2
# 3 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino" 2
# 4 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino" 2
# 5 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino" 2
# 6 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino" 2
# 7 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino" 2
# 8 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino" 2







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
# 25 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
  Motor motor_izq_ade(22, 25, 2, -1, 0x1);
  Motor motor_der_ade(26, 29, 4, -1, 0x1);
  Motor motor_der_atr(27, 28, 5, -1, 0x1);
  Motor motor_izq_atr(23, 24, 3, -1, 0x1);
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
# 41 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
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
  volatile boolean RGB_ready = false; // Variable que indica que se puede leer los nuevos datos del RGB
  int color_anterior = 0; // Variable donde se almacenara el color anterior
  void isr() {RGB_ready = true;} // Funcion de interrupcion
  Adafruit_TCS34725 tcs = Adafruit_TCS34725((0x9C) /**< 240.0ms - 100 cycles - Max Count: 65535 */, TCS34725_GAIN_1X);
  TCS34725 RGB(&tcs, isr);

  //////////////// Declaracion de los actuadores de la pinza ////////////////////
  Motor MotorMini(6,7,1,0x0);
  void bajar_pinza();
  void subir_pinza();

  //////////////// Declaracion del Ultrasonico ////////////////////
  HCSR04 ultrasonico(46, 47);

  //////////////// Declaracion del Servomotor ////////////////////
  Servo servo;




void setup() {
  Serial.begin(115200); // Inicializacion del Serial
  while (! Serial) { delay(1); } // Esperar la Inicializacion del serial
  pinMode(43, 0x1);
  digitalWrite(43, 0x1);
  delay(2000);





    RGB.init_beforeLaser(); // Inicializacion del RGB antes de los laser
    delay(100);
    giroscopio.init(); // Inicializacion del giroscopio
    delay(100);
    dual_laser.init(); // Inicializacion de los sensores laser
    delay(100);
    RGB.init(); // Inicializacion del RGB
    servo_init();

}

void loop() {





    /*dis_sensors.set_position(180);

    dis_sensors.read();

    Serial.print("  freA = "); Serial.print(dis_sensors.dis_freA);

    Serial.print("  freB = "); Serial.print(dis_sensors.dis_freB);

    Serial.print("  atrA = "); Serial.print(dis_sensors.dis_atrA);

    Serial.print("  atrB = "); Serial.print(dis_sensors.dis_atrB);

    Serial.print("  izq1 = "); Serial.print(dis_sensors.dis_izq1);

    Serial.print("  izq2 = "); Serial.print(dis_sensors.dis_izq2);

    Serial.print("  der1 = "); Serial.print(dis_sensors.dis_der1);

    Serial.print("  der2 = "); Serial.println(dis_sensors.dis_der2);*/
# 118 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
    giroscopio.read(); // Lectura en x, y, z
    int z = giroscopio.z(); // Recuperar el valor en z del giroscopio

    if(z > 2 && z <= 5) motores.Move(dis_sensors.position, 2,255); // Si el desfase es pequeño en positivo
    else if(z >= 355 && z < 358) motores.Move(dis_sensors.position,-2,255); // Si el desfase es pequeño en negativo
    else if(z > 5 && z <= 180) motores.turnLeft(100); // Si el desfase es grande en positivo
    else if(z > 180 && z < 355) motores.turnRight(100); // Si el desfase es grande en negativo
    else motores.Move(dis_sensors.position,0,255); // Si no hay desfase avanza

    dis_sensors.read_front_sensors(); // Lee los sensores de distancia del frente
    if((dis_sensors.dis_freA+dis_sensors.dis_freB)/2 <= 10){ // Si los sensores del frente son menor a 10cm
      //FIXME: Comprobar antes de cambiar la direccion que este algunos cm separado el robot de la pared de al frente
      dis_sensors.read_right_sensors(); // Se lee los sensores de la derecha
      dis_sensors.read_left_sensors(); // Se lee los sensores de la derecha

      int prom_der = (dis_sensors.dis_der1+dis_sensors.dis_der2)/2; // Se promedian los sensores de la derecha
      int prom_izq = (dis_sensors.dis_izq1+dis_sensors.dis_izq2)/2; // Se promedian los sensores de la izquierda
      int diff = prom_der-prom_izq; // Se obtiene la diferencia de los dos sensores

      if(diff > 0) dis_sensors.add_angle2position(90); // Si hay salida a la derecha
      else if(diff < 0) dis_sensors.add_angle2position(-90); // Si hay salida a la izquierda
    }

    if (RGB_ready) { // Si el RGB tiene un nuevo valor
      int color = encontrar_color(); // Obtener el color (Blanco(0), Rojo(1), Verde(2) o Azul(3))
      if(color != color_anterior){ // Si el color nuevo es diferente al color anterior
        if(color == 1){ // Si el color es rojo
          dis_sensors.read_right_sensors(); // Se lee los sensores de la derecha
          if(dis_sensors.dis_der2 < 10){ // Si el sensor de la derecha-atras detecta pared
            while (dis_sensors.dis_der2 < 10) { // Avanza hasta que deje de detectar pared
              motores.Move(dis_sensors.position, 0, 255);
              dis_sensors.read_right_sensors();
            }
            motores.stop();
          }
          else if(dis_sensors.dis_der1 < 10){ // Si el sensor de la derecha-adelante detecta pared
            while (dis_sensors.dis_der1 < 10) { // Retrocede hasta que deje de detectar pared
              motores.Move(dis_sensors.position_back, 0, 255);
              dis_sensors.read_right_sensors();
            }
            motores.stop();
          }
          dis_sensors.add_angle2position(90);
        }
        if(color == 3){
          dis_sensors.read_left_sensors(); // Se lee los sensores de la derecha
          if(dis_sensors.dis_izq2 < 10){ // Si el sensor de la derecha-atras detecta pared
            while (dis_sensors.dis_izq2 < 10) { // Avanza hasta que deje de detectar pared
              motores.Move(dis_sensors.position, 0, 255);
              dis_sensors.read_left_sensors();
            }
            motores.stop();
          }
          else if(dis_sensors.dis_izq1 < 10){ // Si el sensor de la derecha-adelante detecta pared
            while (dis_sensors.dis_izq1 < 10) { // Retrocede hasta que deje de detectar pared
              motores.Move(dis_sensors.position_back, 0, 255);
              dis_sensors.read_left_sensors();
            }
            motores.stop();
          }
          dis_sensors.add_angle2position(-90); // Si el color es azul
        }
        color_anterior = color; // Se actualiza el color anterior con el color nuevo
      }
      RGB_ready = false; // Reiniciamos la variable para obtener un nuevo valor del RGB
    }

    /*Serial.print("  freA = "); Serial.print(dis_sensors.dis_freA);

    Serial.print("  freB = "); Serial.print(dis_sensors.dis_freB);    

    Serial.print("  Z = "); Serial.print(z);

    Serial.print("  pos = "); Serial.println(dis_sensors.position);*/
# 191 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
    /*dis_sensors.read();

    Serial.print(dis_sensors.dis_izq1); Serial.print("    "); Serial.println(dis_sensors.dis_izq2);



    if(dis_sensors.dis_izq1 >  8 || dis_sensors.dis_izq2 > 8){

      motores.Move(90, 0, 150);

    }

    else if(dis_sensors.dis_izq1 > dis_sensors.dis_izq2){

      motores.Move(0, 1, 150);

    }

    else if(dis_sensors.dis_izq1 < dis_sensors.dis_izq2){

      motores.Move(0, -1, 150);

    }

    else{

      motores.Move(0, 0, 150);

    }*/
# 211 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
}
# 257 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
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
# 300 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
    MotorMini.Move(0);
  }
  void subir_pinza(){
    MotorMini.Move(-180);
    delay(3000);
    MotorMini.Move(-50);
    servoMove(65, 150, 30);
  }
