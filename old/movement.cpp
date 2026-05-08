#include "movement.h"

int BLACK = 1;
int WHITE = 0;

Movement::Movement(Motor &L, Motor &R)
    : Left_mt(L), Right_mt(R), slow_speed(50), med_speed(125), high_speed(210){}


void Movement::right_balance(){
    analogWrite(Right_mt.pin2, high_speed);
    analogWrite(Right_mt.pin1, 0);
    analogWrite(Left_mt.pin2, high_speed);
    analogWrite(Left_mt.pin1, 0);
}

void Movement::left_balance(){
    analogWrite(Right_mt.pin2, high_speed);
    analogWrite(Right_mt.pin1, 0);
    analogWrite(Left_mt.pin2, high_speed);
    analogWrite(Left_mt.pin1, 0);
}

void Movement::turn_left(){
    Right_mt.ileri(high_speed);
    delay(250);
    Left_mt.geri(high_speed);
    delay(250);
}

void Movement::turn_right(){
    Left_mt.ileri(high_speed);
    delay(250);
    Right_mt.geri(high_speed);
    delay(250);
}

void Movement::go(){
    Left_mt.ileri(high_speed);
    Right_mt.ileri(high_speed);
}

void Movement::stop(){
    Left_mt.dur();
    Right_mt.dur();
}

void Movement::start(int TL, int TR, int LS, int MS, int RS)  {

    if ( LS == BLACK && MS == WHITE  && RS == WHITE) {
        // saga
        left_balance();
    }
    else if (LS ==WHITE  && MS == WHITE  && RS == BLACK) {
        // Sağ sensör siyah gördüğü an sağa dön
        right_balance();
    }
    else if (LS == WHITE && MS == BLACK  && RS == BLACK) {
        // Sağ sensör siyah gördüğü an sağa dön
        right_balance();
    }
    else if (LS == BLACK && MS == BLACK  && RS ==WHITE ) {
        // Solda yol bulundu
        left_balance();
    }
    else {
        // Hepsi beyaz görüyorsa (Çizgiden çıkıldıysa)
        go();
    }
}

