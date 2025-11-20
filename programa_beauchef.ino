#include <QTRSensors.h>
#include "variable.h"

#define NUM_SENSORS 6
#define NUM_SAMPLES_PER_SENSOR 4
#define EMITTER_PIN 11
#define buzzer 10

int  Tp  = 40;
float Ki = 0.015;
float Kd = 7.8;
float Kp = 0.8;
int lim = 70;

QTRSensorsAnalog qtra((unsigned char[]) {
  6, 5, 4, 3, 2, 1
},
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

int posicion;
float error;
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


void setup() {
  Serial.begin(9600);

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

  noTone(10);
  //aqui se activa solo si no fue activado antes
  if(lastde==false&&der<500) {
    tone(10, 1000);
    delay(100);
  }
  lastde=(der<500);
  
  if(lastiz==false&&izq<500){
    tone(10, 1000);
    delay(100);
  }
  lastiz=(izq<500);
  
  VelIzq = (Tp + giro);
  VelDer = (Tp - giro);

  //Motores(VelIzq, VelDer);

  lasterror = error;


}
