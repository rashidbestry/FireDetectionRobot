#include "../include/fan.h"
#include <Arduino.h>

Fan::Fan(int p1) pin1(p1)  {
  pinMode(pin1, INPUT);
}

