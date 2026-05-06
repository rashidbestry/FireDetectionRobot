#include <Arduino.h>

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
const int solPin  = 6;
const int ortaPin = 5;
const int sagPin  = 4;

void setup() {
  sag_Motor.baslat();
  sol_Motor.baslat();
  pinMode(solPin, INPUT);
  pinMode(ortaPin, INPUT);
  pinMode(sagPin, INPUT);
}

void loop() {
  int solVal  = digitalRead(solPin);
  int ortaVal = digitalRead(ortaPin);
  int sagVal  = digitalRead(sagPin);

  if (ortaVal == 1 && solVal == 0 && sagVal == 0) {
    // Sadece orta sensör siyah görüyorsa
    duz_git();
  } 
  else if (solVal == 1) {
    // Sol sensör siyah gördüğü an (orta görse de görmese de) sola dön
    sola_don();
  } 
  else if (sagVal == 1) {
    // Sağ sensör siyah gördüğü an sağa dön
    saga_don();
  } 
  else {
    // Hepsi beyaz görüyorsa (Çizgiden çıkıldıysa)
    dur();
  }
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

void dur(){
    sol_Motor.dur();
    sag_Motor.dur();
}

void duz_git(){
    sol_Motor.ileri();
    sag_Motor.ileri();
}