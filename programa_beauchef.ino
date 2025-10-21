#include <QTRSensors.h>
#include "variable.h"

#define NUM_SENSORS 6
#define NUM_SAMPLES_PER_SENSOR 4
#define EMITTER_PIN 11

int  Tp  = 50;
float Ki = 0.01;
float Kd = 8;
float Kp = 0.8;

int lim = 90;


QTRSensorsAnalog qtra((unsigned char[]) {
  5, 4, 3, 2, 1, 0
},
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

int posicion;
float error;
float lasterror;
float derivada;
int ref = 0;
int giro;
int VelIzq = 0;
int VelDer = 0;
int integral = 0;
void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  inicializarMotores();
  for (int i = 0; i < 200; i++) {
    qtra.calibrate();
    delay(1);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(10);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(10);
  }
}

void loop() {
  //el ultimo booleano de la funcion representa si es linea negra o blanca, si es true es blanca, si es false, es negra

  posicion = qtra.readLine(sensorValues, true, true);

  //revisar como funciona map y maneras de optimizarlo
  posicion = map(posicion, 0, 5000, -lim, lim);

  error = posicion - ref;
  integral = integral + error;
  derivada = error - lasterror;
  giro = Kp * error + Ki * integral + Kd * derivada;

  VelIzq = (Tp + giro);
  VelDer = (Tp - giro);

  Motores(VelIzq, VelDer);

  lasterror = error;


}
