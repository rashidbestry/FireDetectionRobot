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

sagMotor sag_Motor(10, 9);
solMotor sol_Motor(5, 6);
const int solPin  = 0;
const int ortaPin = 1;
const int sagPin  = 2;

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

  if ( solVal == 1 && ortaVal == 0  && sagVal == 0) {
    // saga
    sola();
    delay(25);
  } 
  else if (solVal == 0 && ortaVal == 1  && sagVal == 0) {
    // Sol sensör siyah gördüğü an (orta görse de görmese de) sola dön
    duz_git();
  } 
  else if (solVal == 0 && ortaVal == 0  && sagVal == 1) {
    // Sağ sensör siyah gördüğü an sağa dön
    sag_denge();
    delay(25);
  } 
  else {
    // Hepsi beyaz görüyorsa (Çizgiden çıkıldıysa)
    dur();
  }
}

void saga(){
  sol_Motor.ileri();
}
void sola(){
  sag_Motor.ileri();
}
void sag_denge(){
  analogWrite(9,127);
  analogWrite(10,0);
  analogWrite(6,255);
  analogWrite(5,0);
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