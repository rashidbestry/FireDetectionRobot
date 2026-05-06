#include "../include/engine.h"
#include <Arduino.h>

Motor::Motor(int p1, int p2) : pin1(p1), pin2(p2) {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void Motor::ileri() {
  analogWrite(pin2, 120);
  analogWrite(pin1, 0);
}

void Motor::geri() {
  analogWrite(pin2, 0);
  analogWrite(pin1, 120);
}

void Motor::dur() {
  digitalWrite(pin2, LOW);
  digitalWrite(pin1, LOW);
}