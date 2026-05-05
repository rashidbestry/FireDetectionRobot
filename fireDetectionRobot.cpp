class Motor {
  protected: // Alt sınıfların erişebilmesi için protected
    int pin1;
    int pin2;

  public:
    Motor(int p1, int p2) {
      pin1 = p1;
      pin2 = p2;
    }

    void baslat() {
      pinMode(pin1, OUTPUT); 
      pinMode(pin2, OUTPUT);
    }

    void ileri() {
      digitalWrite(pin2, HIGH);
      digitalWrite(pin1, LOW);
    }

    void geri() {
      digitalWrite(pin2, LOW);
      digitalWrite(pin1, HIGH);
    }

    void dur() {
      digitalWrite(pin2, LOW);
      digitalWrite(pin1, LOW);
    }
}; 

class sagMotor : public Motor {
  public:
    sagMotor(int p1, int p2) : Motor(p1, p2) {}
};

class solMotor : public Motor {
  public:
    solMotor(int p1, int p2) : Motor(p1, p2) {}
};

sagMotor sag_Motor(0, 1);
solMotor sol_Motor(2, 3);

void setup() {
  sag_Motor.baslat();
  sol_Motor.baslat();
}

void loop() {
  sola_don();
  saga_don();
}

void sola_don() {
  sag_Motor.ileri();
  delay(500);
  sag_Motor.dur();
  delay(5000);
}

void saga_don() {
  sol_Motor.ileri();
  delay(500);
  sol_Motor.dur();
  delay(500);
}
