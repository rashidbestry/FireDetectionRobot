#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
class Motor {
  protected: // Alt sınıfların erişebilmesi için protected
    int pin1;
    int pin2;
    

  public:
    Motor(int p1, int p2) {
      pin1 = p1;
      pin2 = p2;
      pinMode(p1, OUTPUT); 
      pinMode(p2, OUTPUT);
    }

    void ileri() {
      analogWrite(pin2, 120);
      analogWrite(pin1, 0);
    }

    void geri() {
      analogWrite(pin2, 0);
      analogWrite(pin1, 120);
    }

    void dur() {
      digitalWrite(pin2, LOW);
      digitalWrite(pin1, LOW);
    }
}; 
#endif