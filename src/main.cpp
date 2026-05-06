#include <Arduino.h>
#include "../include/movement.h"
#include "../include/motor.h"

const int L_SIG_pin  = 2;
const int M_SIG_pin = 3;
const int R_SIG_pin = 4;
const int Fire_SIG = 555;
const int FAN_ENABLE = 455; 
const int BLACK=1;
const int WHITE=0;

Motor Left(5,6);
Motor Right(10,9);

Movement robot(Left,Right);

void setup() {
    pinMode(L_SIG_pin, INPUT);
    pinMode(M_SIG_pin, INPUT);
    pinMode(R_SIG_pin, INPUT);
}

void loop() {
  
  int LS  = digitalRead(L_SIG_pin);
  int MS = digitalRead(M_SIG_pin);
  int RS  = digitalRead(R_SIG_pin);
  

   if ( LS == BLACK && MS == WHITE  && RS == WHITE) {
    // Sağa denge
    robot.left_balance();
  } 
  else if (LS == WHITE && MS == BLACK  && RS == WHITE) {
    // Düz git
    robot.go();
  } 
  else if (LS ==WHITE  && MS == WHITE  && RS == BLACK) {
    // Sola Denge
    robot.right_balance();
  } 
  else if (LS == WHITE && MS == BLACK  && RS == BLACK) {
    // Sağ sensör siyah gördüğü an sağa dön
    robot.stop();
    robot.turn_right();
  }
  else if (LS == BLACK && MS == BLACK  && RS ==WHITE ) {
    // Solda yol bulundu
    robot.stop();
    robot.turn_left();
  }
  else {
    // Hepsi beyaz görüyorsa (Çizgiden çıkıldıysa)
    robot.go();
  }
 
}