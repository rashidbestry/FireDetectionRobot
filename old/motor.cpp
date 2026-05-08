#include "motor.h"
#include <Arduino.h>

Motor::Motor(int p1, int p2) : pin1(p1), pin2(p2) {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void Motor::ileri(int speed) {
  digitalWrite(pin2, 1);
  digitalWrite(pin1, 0);
}

void Motor::geri(int speed) {
  analogWrite(pin2, 0);
  analogWrite(pin1, speed);
}

void Motor::dur() {
  digitalWrite(pin2, LOW);
  digitalWrite(pin1, LOW);
}