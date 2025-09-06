//MINISUMO
byte sensores;
//////////////////////SENSORES
const int sensorIzq = 4; //SENSOR PRESENCIA IZQ - D7
const int sensorDer = 2; //SENSOR PRESENCIA DER - D2
const int qtr_cen = A3; //SENSOR DE LINEA DER - A3
///////////////////////MOTORES
const int PWMA = A0; // ANALOGO 0
const int AN1 = 3; // DIGITAL 3  DERECHA / MOTOR 1
const int AN2 = 6; // DIGITAL 6  DERECHA / MOTOR 1
const int BN1 = 9; // DIGITAL 9  IZQUIERDA / MOTOR 2
const int BN2 = 5; // DIGITAL 5  IZQUIERDA / MOTOR 2
const int PWMB = A1; // ANALOGO 1
///////////////////////ACTIVADOR
const int ACTIVADOR = 10;
////////////////////////BOTON (TAMBIEN SE AGREGO APENAS)
const int buttonPin = 13;
int buttonState = 0;
int lastButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int select = 0;
////////////////////////VARIABLES
int valorSenIzq = 0;
int valorSenDer = 0;
int valQTR_Cen = 0;
////////////////////////CONVERSIÓN A BYTES
byte cen = 0;
bool IZQ = false , DER = true; 
int atk = 0;
int centro = 0;
int espera = 0;
int espera2 = 0;
int espera3 = 0;
int search = 0;

void setup() {
    pinMode(AN1, OUTPUT);
    pinMode(AN2, OUTPUT);
    pinMode(BN1, OUTPUT);
    pinMode(BN2, OUTPUT);
    pinMode(sensorIzq, INPUT);
    pinMode(sensorDer, INPUT);
    digitalWrite(AN1, LOW);
    digitalWrite(AN2, LOW);
    digitalWrite(BN1, LOW);
    digitalWrite(BN2, LOW);
    pinMode(ACTIVADOR, INPUT);
    delay(1000);
}

void lecturas() {
    valorSenIzq = digitalRead(sensorIzq);
    valorSenDer = digitalRead(sensorDer);
    valQTR_Cen = analogRead(qtr_cen);

    if (valorSenIzq == 1) { izq_1 = 0B00010000;} 
    else { izq_1 = 0B00000000; }
    if (valorSenDer == 1) { der_1 = 0B00000100;} 
    else { der_1 = 0B00000000; }
    sensores = der_1 + izq_1;
    if(sensores != 0B00000000){ search = 0; }
    else{ search == 1; }
}

void loop() {
  digitalWrite(AN1, LOW);
  digitalWrite(AN2, LOW);
  analogWrite(PWMA, 0);
  digitalWrite(BN1, LOW);
  digitalWrite(BN2, LOW);
  analogWrite(PWMB, 0);

  if(digitalRead(ACTIVADOR) == LOW){
    int reading = digitalRead(buttonPin);
    if(reading != lastButtonState){ lastDebounceTime = millis(); }
    if((millis() - lastDebounceTime) > debounceDelay){
      if(reading !=buttonState){
        buttonState = reading;
        if(buttonState == HIGH){ select++; }
      }
    }
    lastButtonState = reading;
    if(select > 3){ select = 0; }
  }

  if (digitalRead(ACTIVADOR) == HIGH) {
    estrategias();
  }
  else { 
    detener(1); 
  }
}

void estrategias(){
  switch(select){
    case 1:
      izquierda(90, 90, 220);
      adelante(90, (90*0.9), 150);
      derecha(90, 90, 150);
      batalla();
    break;
    case 2:
      derecha(90, 90, 190);
      adelante(90, (90*0.9), 100);
      izquierda(90, 90, 220);
      batalla();
    break;
    case 3:
      do{
          //GRIO DERECHA
          digitalWrite(BN2, LOW);
          analogWrite(BN1, 50);
          digitalWrite(AN2, LOW);
          analogWrite(AN1, 70); 
          delay(80);
          //GIRO IZQUIERDA
          digitalWrite(BN1, LOW);
          analogWrite(BN2, PWMB); 
          digitalWrite(AN1, LOW);
          analogWrite(AN2, PWMA);
          delay(80);
      }while(); 
      batalla();
    break;
    default:
     batalla();
    break;
  }
}

void batalla() {
  do {
    lecturas();
    switch (sensores) {
      case 0B00000100:
        DER = true;
        IZQ = false;
        derecha(80, 80, 8);
        detener(5);
      break;
      case 0B00010000:
        DER = false;
        IZQ = true;
        izquierda(80, 80, 8);
        detener(5);
      break;
      case 0B00010100:
        if(espera2 == 0){
          detener(1);
          espera++;
          if(espera > 500){ adelante(160, 140, 40); espera3++; }
          if(espera3 > 2){ espera2 = 1; }
        }else{
          adelante(atk, atk+2, 1);
          centro++;
          delay(1); 
          if( centro>2 ){atk++; centro = 0;}
          if( atk > 255){ atk = 253; } // se reduce velocidad máxima de ataque
        }
      break;
      default:
        adelante(100, 80, 1);
        if (valQTR_Cen == 0) {
          if(IZQ == true){
            atras(90, 90, 100);
            izquierda(90, 90, 450);
          }
          if(DER == true){
            atras(90, 90, 350);
            derecha(90, 90, 450);
          }
        }
      break;
    }
    if (valQTR_Cen < 50) {
      if(IZQ == true){
        atras(90, 90, 350);
        izquierda(90, 90, 300);
      }
      if(DER == true){
        atras(90, 90, 350);
        derecha(90, 90, 300);
      }
    }
  } while (digitalRead(ACTIVADOR) == HIGH);
  detener(1);
}

void adelante(byte PWMA,byte PWMB,int T) {
  digitalWrite(AN1, LOW);
  analogWrite(AN2, PWMA);
  digitalWrite(BN2, LOW);
  analogWrite(BN1, PWMB);
  delay (T);
}

void atras(byte PWMA,byte PWMB,int T) {
  digitalWrite(AN2, LOW);
  analogWrite(AN1, PWMA); 
  digitalWrite(BN1, LOW);
  analogWrite(BN2, PWMB); 
  delay(T); 
}

void izquierda(byte PWMA,byte PWMB,int T) {
  digitalWrite(AN1, LOW);
  analogWrite(AN2, PWMA);
  digitalWrite(BN1, LOW);
  analogWrite(BN2, PWMB);
  delay (T);
}

void derecha(byte PWMA,byte PWMB,int T) {
  digitalWrite(AN2, LOW);
  analogWrite(AN1, PWMA);
  digitalWrite(BN2, LOW);
  analogWrite(BN1, PWMB); 
  delay (T); 
}

void detener(int T) {
  digitalWrite(AN2, LOW);
  analogWrite(AN1, 0); 
  digitalWrite(BN1, LOW);
  analogWrite(BN2, 0);
  delay (T);
}