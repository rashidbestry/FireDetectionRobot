#include <Arduino.h>
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "engine.h"

class Movement{
    protected: // Alt sınıfların erişebilmesi için protected
        Motor &Left_mt;
        Motor &Right_mt;

        int slow_speed=50;
        int med_speed=125;
        int high_speed=210;
    
    public:
       Movement(Motor &L, Motor &R) : Left_mt(L), Right_mt(R) {}

        void right_balance(){}
        void left_balance(){}
        void turn_left(){}
        void turn_right(){}
        void go(){}
        void stop(){}
};
#endif

    
    
    
    
    
    