#include <QTRSensors.h>
#include "variable.h"

#define NUM_SENSORS 6
#define NUM_SAMPLES_PER_SENSOR 4
#define EMITTER_PIN 11
#define buzzer 10

int TpMax = 60;
int TpMin = 25;

int TpB=60;

int  Tp  = 50;
float Ki = 0.01;
float Kd = 8.5;
float Kp = 0.4;
int lim = 100;
//cosas buzzer
bool buzzerOn = false;
unsigned long buzzerStart = 0;
int buzzerDuration = 50;
QTRSensorsAnalog qtra((unsigned char[]) {
  6, 5, 4, 3, 2, 1
},
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

int posicion;
float error=0;
float lasterror;
float derivada=0;
int ref = 0;
int giro;
int VelIzq = 0;
int VelDer = 0;
int integral = 0;
int demin=1000;
int demax=0;
int izmin=1000;
int izmax=0;
int izq=0;
int der=0;
bool lastde=false;
bool lastiz=false;
bool contaiz=false;
bool izprendido=false;
bool deprendido=false;
char destop=0;
void setup() {
  while(!digitalRead(2))
      {}
  delay(200);
  Motores(-20,20);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  inicializarMotores();
  for (int i = 0; i < 100; i++) {
    qtra.calibrate();
    izq=analogRead(0);
    der=analogRead(7);
    if (izq < izmin) izmin = izq;
    if (izq > izmax) izmax = izq;
    if (der < demin) demin = der;
    if (der > demax) demax = der;
    
    delay(1);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(10);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(10);
  }
  Motores(0,0);
  while(!digitalRead(2))
      {}
  delay(200);
}

void loop() {
  
  izq=analogRead(7);
  der=analogRead(0);

  der=map(der,demin,demax,0,1000);
  izq=map(izq,izmin,izmax,0,1000);
  
  //programar sensores laterales wa wa...
  //el ultimo booleano de la funcion representa si es linea negra o blanca, si es true es blanca, si es false, es negra
  posicion = qtra.readLine(sensorValues, true, true);  
  //revisar como funciona map y maneras de optimizarlo
  posicion = map(posicion, 0, 5000, -lim, lim);
  error = posicion - ref;
  integral = integral + error;
  derivada= error -lasterror;
  giro = Kp * error + Ki * integral + Kd * derivada;
    //aqui se activa solo si no fue activado antes
    if((lastde==false&&der<500)) {
      tone(10, 1000);
      deprendido=true;
      buzzerOn = true;
      buzzerStart = millis();
    }
    lastde=(der<500);
    
    if((lastiz==false&&izq<500)){
      tone(10, 1000);
      izprendido=true;
      buzzerOn = true;
      buzzerStart = millis();
    }
    
    lastiz=(izq<500);
    
    VelIzq = (Tp + giro);
    VelDer = (Tp - giro);
  
    Motores(VelIzq, VelDer);
  
  
  lasterror = error;
  if (buzzerOn && millis() - buzzerStart >= buzzerDuration) {
    if(izprendido&&deprendido){
      izprendido=false;
      deprendido=false;
    }
    if (izprendido){
      /*
      int absError = abs(error);
      absError = constrain(absError, 0, lim);
      
      float factor = (float)absError / lim;
      Tp = TpMax - factor * (TpMax - TpMin);
      */
    }
    noTone(buzzer);
    buzzerOn = false;
    
    if (deprendido){
      destop+=1;
    }
    if (destop==2){
      destop=0;
      Motores(0,0);
      Tp=TpB;
      while(!digitalRead(2))
      {}
    }
  }

}
