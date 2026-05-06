#include <Arduino.h>
#include "movement.h"
#include "engine.h"

const int L_S_pin  = 2;
const int M_S_pin = 3;
const int R_S_pin = 4;
const int BLACK=1;
const int WHITE=0;


Motor Left(5,6);
Motor Right(10,9);

Movement robot(Left,Right);

void setup() {
    pinMode(L_S_pin, INPUT);
    pinMode(M_S_pin, INPUT);
    pinMode(R_S_pin, INPUT);
}

void loop() {
  
  int LS  = digitalRead(L_S_pin);
  int MS = digitalRead(M_S_pin);
  int RS  = digitalRead(R_S_pin);
  

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