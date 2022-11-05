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
  void isr() {
    RGB_ready = true;
  } // Funcion de interrupcion
  Adafruit_TCS34725 tcs = Adafruit_TCS34725((0x6A) /**< 360.0ms - 150 cycles - Max Count: 65535 */, TCS34725_GAIN_1X);
  TCS34725 RGB(&tcs, isr);

  //////////////// Declaracion de los actuadores de la pinza ////////////////////
  Motor MotorMini(6,7,1,0x0);
  void bajar_pinza();
  void subir_pinza();

  //////////////// Declaracion del Ultrasonico ////////////////////
  HCSR04 ultrasonico(46, 47);

  //////////////// Declaracion del Servomotor ////////////////////
  Servo servo;

  int contador_paredes; //Contador para comprobar que no esta pegado a las paredes
  int x_inicial;
  int y_inicial;
  bool piramide;
  bool es_subida = false;
  int z, x, y;
  int estado = 2;



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

    giroscopio.read(); // Lectura en x, y, z
    x_inicial = giroscopio.x();
    y_inicial = giroscopio.y();

}

void loop() {





    while(estado == 0){
      giroscopio.read(); // Lectura en x, y, z
      z = giroscopio.z(); // Recuperar el valor en z del giroscopio
      x = giroscopio.x() - x_inicial; // Recuperar el valor en x del giroscopio
      y = giroscopio.y() - y_inicial; // Recuperar el valor en y del giroscopio

      avanzar_gyro();
      esquivar_pared_frente();


      if(RGB_ready) { // Si el RGB tiene un nuevo valor
        int color = encontrar_color(); // Obtener el color (Blanco(0), Rojo(1), Verde(2) o Azul(3))
        //Serial.print("color = "); Serial.println(color);
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
            (es_subida) ? dis_sensors.add_angle2position(-90) : dis_sensors.add_angle2position(90);
            //Serial.println("Derecha");
            //motores.stop(); delay(1000);
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
            (es_subida) ? dis_sensors.add_angle2position(90) : dis_sensors.add_angle2position(-90); // Si el color es azul
            //motores.stop(); delay(1000);
            //Serial.println("Izquierda");
          }
          color_anterior = color; // Se actualiza el color anterior con el color nuevo
        }
        RGB_ready = false; // Reiniciamos la variable para obtener un nuevo valor del RGB
      }

      comprobar_dis_paredes();

      if(x < -2){
        estado = 1; //piramide = 1;
      }
      else if(x > 2){
        motores.Move(dis_sensors.position_back,0,255);
        delay(200);
        while(1){
          giroscopio.read(); z = giroscopio.z();
          if(z < 175) motores.turnRight(150);
          else if(z > 185) motores.turnLeft(150);
          else break;
        }
        giroscopio.set_z_offset(180);
        dis_sensors.add_angle2position(180);
        estado = 1; //piramide = 1;
      }
      else if(y < -2){
        motores.Move(dis_sensors.position_back,0,255);
        delay(200);
        while (1){
          giroscopio.read(); z = giroscopio.z();
          if(z < 270) motores.turnRight(150);
          else if(z > 280) motores.turnLeft(150);
          else break;
        }
        giroscopio.set_z_offset(270);
        dis_sensors.add_angle2position(270);
        estado = 1; //piramide = 1;
      }
      else if(y > 2){
        motores.Move(dis_sensors.position_back,0,255);
        delay(200);
        while (1){
          giroscopio.read(); z = giroscopio.z();
          if(z < 85) motores.turnRight(150);
          else if(z > 95) motores.turnLeft(150);
          else break;
        }
        giroscopio.set_z_offset(90);
        dis_sensors.add_angle2position(90);
        estado = 1; //piramide = 1;
      }
    }

    while(estado == 1){
      for(int vueltas = 0 ; vueltas < 7 ; vueltas++){
        dis_sensors.set_position(0);
        motores.Move(dis_sensors.position, 0, 255);
        delay(1000);
        bool giro = 0;
        while(giro == 0){
          giroscopio.read();
          x = giroscopio.x();
          //Serial.print(x);
          if(x >= 2){
            motores.stop();
            while (1){
              giroscopio.read(); z = giroscopio.z();
              if(z < 265) motores.turnRight(150);
              else if(z > 275) motores.turnLeft(150);
              else break;
            }
            giroscopio.set_z_offset(270);
            dis_sensors.add_angle2position(270);
            motores.stop();
            giro = 1;
          }
          else motores.Move(dis_sensors.position,0,255);
        }
      }
      estado = 2;
    }
    int superior_paredes = 0;
    while(estado == 2){
      giroscopio.read();
      z = giroscopio.z();
      if(superior_paredes == 2){
        while (1){
          giroscopio.read(); z = giroscopio.z();
          if(z < 175) motores.turnRight(150);
          else if(z > 185) motores.turnLeft(150);
          else break;
        }
        giroscopio.set_z_offset(180);
        dis_sensors.add_angle2position(180);
        motores.stop();
        pinza();
        while (1){
          giroscopio.read(); z = giroscopio.z();
          if(z < 175) motores.turnRight(150);
          else if(z > 185) motores.turnLeft(150);
          else break;
        }
        giroscopio.set_z_offset(180);
        //dis_sensors.add_angle2position(180);
        motores.stop();
        estado = 3;
        break;
      }
      else if(esquivar_pared_frente()){
        superior_paredes++;
      }
      else{
        avanzar_gyro();
        comprobar_dis_paredes();
      }
    }
    superior_paredes = 0;
    while(estado == 3){
      giroscopio.read(); z = giroscopio.z();
      if(superior_paredes == 2){
        motores.stop();
        estado = 4;
      }
      else if(esquivar_pared_frente()){
        superior_paredes++;
      }
      else{
        avanzar_gyro();
        comprobar_dis_paredes();
      }
    }
    while(estado == 4){
      motores.Move(dis_sensors.position,0,255);
      delay(1000);
      estado = 0;
    }

}
# 354 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
  int encontrar_color(){
    int H, S, V;
      RGB.get_HSV(&H,&S,&V);

      Serial.print("H = "); Serial.println(H);

      if(H < 14 || H > 350) return 1; //Serial.print("Rojo");
      else if(H < 48) return 0; //Serial.print("Blanco");
      else if(H < 85) return 2; //Serial.print("Verde"); //125
      return 3; //Serial.print("Azul");

  }

  void servoMove(int ang_inicial, int ang_final, int time){
    int diff_ang = ang_final - ang_inicial;
    if(diff_ang > 0){
      for(int i = ang_inicial; i <= ang_final ; i++ ){
        servo.write(i);
        //Serial.println(i);
        delay(time);
      }
    }
    else{
      for(int i = ang_inicial; i >= ang_final ; i-- ){
        servo.write(i);
        //Serial.println(i);
        delay(time);
      }
    }
  }
  void servo_init(){
    servo.attach(11);
    MotorMini.Move(-255);
    delay(1000);
    MotorMini.Move(-100);
    servo.write(150);
  }

  void bajar_pinza(){
    servoMove(150, 65, 30);
    MotorMini.Move(180);
    delay(1000);
    /*MotorMini.Move(-10);

    delay(100);*/
# 398 "c:\\Users\\angil\\OneDrive\\Desktop\\RoboSTEM\\RoboSTEM.ino"
    MotorMini.Move(0);
  }

  void subir_pinza(){
    MotorMini.Move(-120);
    delay(2000);
    dis_sensors.read_back_sensors();
    while((dis_sensors.dis_atrA + dis_sensors.dis_atrB)/2 > 10){
      dis_sensors.read_back_sensors();
      motores.Move(dis_sensors.position_back, 0, 255);
    }
    motores.stop();
    MotorMini.Move(-255);
    delay(3000);
    MotorMini.Move(-50);
    servoMove(65, 150, 30);
  }

  void pinza(){
    bajar_pinza();
    delay(1000);
    motores.Move(dis_sensors.position,0,50);
    delay(3000);
    motores.stop();
    delay(1000);
    subir_pinza();
  }

  void avanzar_gyro(){
    if(z > 2 && z <= 5) motores.Move(dis_sensors.position, 0.5,255); // Si el desfase es pequeño en positivo
    else if(z >= 355 && z < 358) motores.Move(dis_sensors.position,-0.5,255); // Si el desfase es pequeño en negativo
    else if(z > 5 && z <= 180) motores.turnLeft(100); // Si el desfase es grande en positivo
    else if(z > 180 && z < 355) motores.turnRight(100); // Si el desfase es grande en negativo
    else motores.Move(dis_sensors.position,0,255); // Si no hay desfase avanza
  }

  bool esquivar_pared_frente(){
    dis_sensors.read_front_sensors(); // Lee los sensores de distancia del frente
    if((dis_sensors.dis_freA+dis_sensors.dis_freB)/2 <= 10){ // Si los sensores del frente son menor a 10cm
      dis_sensors.read_right_sensors(); // Se lee los sensores de la derecha
      dis_sensors.read_left_sensors(); // Se lee los sensores de la derecha

      int prom_der = (dis_sensors.dis_der1+dis_sensors.dis_der2)/2; // Se promedian los sensores de la derecha
      int prom_izq = (dis_sensors.dis_izq1+dis_sensors.dis_izq2)/2; // Se promedian los sensores de la izquierda
      int diff = prom_der-prom_izq; // Se obtiene la diferencia de los dos sensores

      if(diff > 0) {dis_sensors.add_angle2position(90); return true;} // Si hay salida a la derecha
      else if(diff < 0) {dis_sensors.add_angle2position(-90); return true;} // Si hay salida a la izquierda
    }
    else return false;
  }

  void comprobar_dis_paredes(){
    if(contador_paredes >= 2){ // Cada 10 iteraciones del codigo comprueba si no se ha pegado a alguna pared
        dis_sensors.read_right_sensors(); // Se lee los sensores de la derecha
        dis_sensors.read_left_sensors(); // Se lee los sensores de la izquierda

        int prom_der = (dis_sensors.dis_der1+dis_sensors.dis_der2)/2; // Se promedian los sensores de la derecha
        int prom_izq = (dis_sensors.dis_izq1+dis_sensors.dis_izq2)/2; // Se promedian los sensores de la izquierda

        if(prom_der <= 4){ // Si los sensores de la derecha detectan que esta cerca de la pared
          while (prom_der <= 4 ){ // Hasta que detecte un distancia sea mayor a 6cm
            motores.Move(dis_sensors.position_left, 0, 150); // Se aleja de la pared
            dis_sensors.read_right_sensors();
            prom_der = (dis_sensors.dis_der1+dis_sensors.dis_der2)/2;
            //Serial.println(dis_sensors.position_left);
          }
          motores.stop();
        }
        else if(prom_izq <= 4){ // Si los sensores de la izquierda detectan que esta cerca de la pared
          while (prom_izq <= 4 ){ // Hasta que detecte un distancia sea mayor a 6cm
            motores.Move(dis_sensors.position_right, 0, 150); // Se aleja de la pared
            dis_sensors.read_left_sensors();
            prom_izq = (dis_sensors.dis_izq1+dis_sensors.dis_izq2)/2;
            //Serial.println(dis_sensors.position_right);
          }
          motores.stop();
        }
        contador_paredes = 0; // Reinicia la variable del contador
      }
      else contador_paredes++; // Si aun no llega al valor se realiza un conteo
  }
