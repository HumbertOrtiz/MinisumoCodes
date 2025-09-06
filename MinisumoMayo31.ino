byte sensores;
//////////////////BOTONES Y ACTIVADOR
int ACTIVADOR = 8;
int Dswitch_1 = A7;
int Dswitch_2 = A6;
/////////////////SENSORES
int sensorD = A0;
int sensorI = A4;
int qtr = A1;
/////////////MOTORES A Y B 
int PWMA = 3;
int MA1 = 5;
int MA2 = 4;
int PWMB = 11;
int MB1 = 6;
int MB2 = 7;
int STBY = 12;
//int ARR = 8;
///////////////////////////// VALORES DE LOS SENSORES
int sD = 0, sI = 0, qtrL = 0;

byte sD_1 = 0, sI_1 = 0, qtrL_1 = 0; 

boolean DER = false;
boolean IZQ = false;

/////////////////////////////DIP SWITCH
int dip1 = 0, dip2 = 0;
int dips = 0;


int atk = 0;
int centro = 0;
int espera = 0;
int espera2 = 0;
int espera3 = 0;

void setup() {
  pinMode(ACTIVADOR, INPUT);

  pinMode(Dswitch_1, INPUT_PULLUP);
  pinMode(Dswitch_2, INPUT_PULLUP);

  pinMode(sensorD, INPUT);
  pinMode(sensorI, INPUT);
  pinMode(qtr, INPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  pinMode(PWMA, OUTPUT);
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);

  digitalWrite(MA1, LOW);
  digitalWrite(MA2, LOW);
  digitalWrite(MB1, LOW);
  digitalWrite(MB2, LOW);

  delay(1000);
}

void lecturas(){
  sD = digitalRead(sensorD);
  sI = digitalRead(sensorI);
  qtrL = analogRead(qtr);

  if( sD == 0){ sD_1 = 0B00000001; }
  else{ sD_1 = 0B00000000; }

  if( sI == 0){ sI_1 = 0B00000010; }
  else{ sI_1 = 0B00000000; }

  if(qtrL <= 150){ qtrL_1 = 0B00000100;}
  else{ qtrL_1 = 0B00000000;}

  sensores = sD_1 + sI_1 + qtrL_1;
}

void loop(){
  if(digitalRead(ACTIVADOR) == LOW){
    estrategias();
  }
  if(digitalRead(ACTIVADOR) == HIGH){
    switch ( dips ){
    case 1:
      //Espaldas
      //
      batalla();
    break;

    case 2:
      //función incial
      //
      batalla();
    break;

    case 3:
      //función incial
      //
      batalla();
    break;

    default:
      batalla();
    break;
  }
  }else{
    detener(1);
  }
}

void estrategias(){
  // Invertimos porque INPUT_PULLUP: 0 = ON
  dip1 = digitalRead(Dswitch_1); // MSB
  dip2 = digitalRead(Dswitch_2); // LSB 
  // Convertimos binario a decimal (2 bits)
}

void batalla(){
  do{
    lecturas();
    switch( sensores ){
      case 0B00000001: // DETECTA DERECHA
        DER = true;
        IZQ = false;
        derecha(180, 180, 5);
        detener(10);
      break;

      case 0B00000010: // DETECTA IZQUIERDA
        IZQ = true;
        DER = false;
        izquierda(180, 180, 5);
        detener(10);
      break;

      case 0B00000011:
        //adelante(180, 180, 5);
        if(espera2 == 0){
          detener(1);
          espera++;
          if(espera>500){adelante(120, 122, 60);espera=0;espera3++;}
          if(espera3>2){espera2=1;}
        }else{
          adelante(atk, atk+2, 1);
          centro++;
          delay(1);
          if(centro>2){atk++;centro=0;}
          if(atk>252){atk=253;}
        }
      break;

      case 0B00000100:
          atras(250, 250, 150);
          derecha(190, 240, 250);

      default:
        adelante(130, 130, 10);
          /*if( qtrL <= 50 ){
            atras(250, 250, 150);
            derecha(190, 240, 150);
          }*/
      break;
    }
      /*if( qtrL <= 50 ){
        atras(250, 250, 150);
        derecha(190, 240, 150);
      }*/
  }while(digitalRead(ACTIVADOR) == HIGH);
  detener(1);
}

void adelante(byte PWM1, byte PWM2, int T){
  digitalWrite(MA1, LOW); //MOTOR IZQUIERDO
  digitalWrite(MA2, HIGH);
  analogWrite(PWMA, PWM1);

  digitalWrite(MB1, HIGH); // MOTOR DERECHO
  digitalWrite(MB2, LOW);
  analogWrite(PWMB, PWM2);
  delay(T);
}
void izquierda(byte PWM1, byte PWM2, int T){
  digitalWrite(MA1, HIGH);
  digitalWrite(MA2, LOW);
  analogWrite(PWMA, PWM1);

  digitalWrite(MB1, HIGH);
  digitalWrite(MB2, LOW);
  analogWrite(PWMB, PWM2);
  delay(T);
}
void derecha(byte PWM1, byte PWM2, int T){
  digitalWrite(MA1, LOW);
  digitalWrite(MA2, HIGH);
  analogWrite(PWMA, PWM1);

  digitalWrite(MB1, LOW);
  digitalWrite(MB2, HIGH);
  analogWrite(PWMB, PWM2);
  delay(T);
}
void atras(byte PWM1, byte PWM2, int T){
  digitalWrite(MA1, HIGH);
  digitalWrite(MA2, LOW);
  analogWrite(PWMA, PWM1);

  digitalWrite(MB1, LOW);
  digitalWrite(MB2, HIGH);
  analogWrite(PWMB, PWM2);
  delay(T);
}
void detener(int T){
  digitalWrite(MA1, LOW);
  digitalWrite(MA2, LOW);
  analogWrite(PWMA, 0);

  digitalWrite(MB1, LOW);
  digitalWrite(MB2, LOW);
  analogWrite(PWMB, 0);
  delay(T);
}