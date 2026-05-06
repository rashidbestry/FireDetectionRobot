#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>
#include "motor.h"

class Movement {
protected:
    Motor &Left_mt;
    Motor &Right_mt;
    int slow_speed;
    int med_speed;
    int high_speed;
public:
    Movement(Motor &L, Motor &R);
    void right_balance();
    void left_balance();
    void turn_left();
    void turn_right();
    void go();
    void stop();
};

#endif

    
    
    
    
    
    