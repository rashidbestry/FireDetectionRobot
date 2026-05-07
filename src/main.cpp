#include <Arduino.h>
#include "../include/movement.h"
#include "../include/motor.h"

const int TURN_LEFT_SIG = 555;          //Robot sola döndür
const int TURN_RIGHT_SIG = 666;         //Robot sağa döndür
const int LEFT_SIG  = 2;           // Orta çizgi SOL sensör
const int MEDIUM_SIG = 3;            // Orta çizgi ORTA sensör
const int RIGHT_SIG = 4;            // Orta çizgi SAG sensör
const int FIRE_SIG = 555;           // Yangin Sensör
const int FAN_ENABLE = 455;         // Fan çaliştirma sensör
const int FRONT_TRIG = 435;         // Mesafe sensör sinyal GÖNDERME
const int FRONT_ECHO = 666;         // Mesafe sensör sinyal GECİKME
const int BACK_TRIG = 333;          // Mesafe sensör sinyal GECİKME
const int BACK_ECHO = 666;          // Mesafe sensör sinyal GECİKME
const int BLACK=1;                  // Siyah Çizgi
const int WHITE=0;                  // Beyaz Alan

Motor Left(5,6);                    // SOL MOTOR NESNE
Motor Right(10,9);                  // SAG MOTOR NESNE

Movement robot(Left,Right);         // ROBOT HAREKET NESNE

void setup() {
    pinMode(TURN_LEFT_SIG, INPUT);
    pinMode(TURN_RIGHT_SIG, INPUT);
    pinMode(LEFT_SIG, INPUT);
    pinMode(MEDIUM_SIG, INPUT);
    pinMode(RIGHT_SIG, INPUT);
    pinMode(FIRE_SIG, INPUT);
    pinMode(FAN_ENABLE, OUTPUT);
    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);
    pinMode(BACK_TRIG, OUTPUT);
    pinMode(BACK_ECHO, INPUT);
}

void loop() {
  
  int TL = digitalRead(TURN_LEFT_SIG);
  int TR = digitalRead(TURN_RIGHT_SIG);
  int LS  = digitalRead(LEFT_SIG);
  int MS = digitalRead(MEDIUM_SIG);
  int RS  = digitalRead(RIGHT_SIG);

  
  if (digitalRead(FIRE_SIG) == 1) {    // Yangin sensoru aktif olursa Fan çalişacak
    digitalWrite(FAN_ENABLE,HIGH);
  }

  robot.start(TL,TR,LS,MS,RS);                          // Robot başlatma noktasi
  
}