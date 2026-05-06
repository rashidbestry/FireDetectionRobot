#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
class Motor {
  public: // Alt sınıfların erişebilmesi için protected
    int pin1;
    int pin2;
    

  public:
    Motor(int p1, int p2) {
      pin1 = p1;
      pin2 = p2;
    }

    void ileri() {}

    void geri() {}

    void dur() {}
}; 
#endif