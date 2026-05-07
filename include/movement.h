#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Arduino.h>
#include "motor.h"

class Movement {
public:
    Motor &Left_mt;
    Motor &Right_mt;
    int slow_speed;
    int med_speed;
    int high_speed;
public:
    Movement(Motor &L, Motor &R);
    static void right_balance();
    static void left_balance();
    static void turn_left();
    static void turn_right();
    static void go();
    static void stop();
    void start();
};

#endif

    
    
    
    
    
    