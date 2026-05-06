#include "../include/movement.h"

Movement::Movement(Motor &L, Motor &R)
  : Left_mt(L), Right_mt(R), slow_speed(50), med_speed(125), high_speed(210) {}

void Movement::right_balance(){
    analogWrite(Right_mt.pin2, slow_speed);
    analogWrite(Right_mt.pin1, 0);
    analogWrite(Left_mt.pin2, high_speed);
    analogWrite(Left_mt.pin1, 0);
}

void Movement::left_balance(){
    analogWrite(Right_mt.pin2, high_speed);
    analogWrite(Right_mt.pin1, 0);
    analogWrite(Left_mt.pin2, slow_speed + 30);
    analogWrite(Left_mt.pin1, 0);
}

void Movement::turn_left(){
    Right_mt.ileri();
    delay(250);
    Left_mt.geri();
    delay(250);
}

void Movement::turn_right(){
    Left_mt.ileri();
    delay(250);
    Right_mt.geri();
    delay(250);
}

void Movement::go(){
    Left_mt.ileri();
    Right_mt.ileri();
}

void Movement::stop(){
    Left_mt.dur();
    Right_mt.dur();
}
