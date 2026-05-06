#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
  public:
    int pin1;
    int pin2;
    Motor(int p1, int p2);
    void ileri(int speed);
    void geri(int speed);
    void dur();
};

#endif