#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>

class Motor {
  public:
    int pin1;
    int pin2;
    Motor(int p1, int p2);
    void ileri();
    void geri();
    void dur();
};

#endif