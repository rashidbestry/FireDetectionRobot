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

        void right_balance(){
            analogWrite(Right_mt.pin2,slow_speed);
            analogWrite(Right_mt.pin1,0);
            analogWrite(Left_mt.pin2,high_speed);
            analogWrite(Left_mt.pin1,0);
        }
        void left_balance(){
            analogWrite(Right_mt.pin2,high_speed);
            analogWrite(Right_mt.pin1,0);
            analogWrite(Left_mt.pin2,slow_speed+30);
            analogWrite(Left_mt.pin1,0);
        }
        void turn_left(){
            Right_mt.ileri();
            delay(250);
            Left_mt.geri();
            delay(250);
        }
        void turn_right(){
            Left_mt.ileri();
            delay(250);
            Right_mt.geri();
            delay(250);
        }
        void go(){
            Left_mt.ileri();
            Right_mt.ileri();
        }
        void stop(){
            Left_mt.dur();
            Right_mt.dur();
    }
};
#endif

    
    
    
    
    
    