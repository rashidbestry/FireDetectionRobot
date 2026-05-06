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

    void ileri(int speed) {
      analogWrite(pin2, speed);
      analogWrite(pin1, 0);
    }

    void geri() {
      analogWrite(pin2, 0);
      analogWrite(pin1, 125);
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
// sensor pin 
const int L_S_pin  = 2;
const int M_S_pin = 3;
const int R_S_pin = 4;

// 
const int slow_speed=50;
const int med_speed=125;
const int high_speed=210;
void setup() {
  sag_Motor.baslat();
  sol_Motor.baslat();
  pinMode(L_S_pin, INPUT);
  pinMode(M_S_pin, INPUT);
  pinMode(R_S_pin, INPUT);
}

void loop() {
  int LS  = digitalRead(L_S_pin);
  int MS = digitalRead(M_S_pin);
  int RS  = digitalRead(R_S_pin);

  // analogWrite(9,255);
  // analogWrite(10,0);
  // analogWrite(6,255);
  // analogWrite(5,0);
  int BLACK=1;
  int WHITE=0;

 
  if ( LS == BLACK && MS == WHITE  && RS == WHITE) {
    // saga
    sol_denge();
  } 
  else if (LS ==WHITE  && MS == WHITE  && RS == BLACK) {
    // Sağ sensör siyah gördüğü an sağa dön
    sag_denge();
  } 
  else if (LS == WHITE && MS == BLACK  && RS == BLACK) {
    // Sağ sensör siyah gördüğü an sağa dön
    sag_denge();
  }
  else if (LS == BLACK && MS == BLACK  && RS ==WHITE ) {
    // Solda yol bulundu
    sol_denge();
  }
  else {
    // Hepsi beyaz görüyorsa (Çizgiden çıkıldıysa)
    duz_git();
  }
}

void sag_denge(){
  analogWrite(9,med_speed);
  analogWrite(10,0);
  analogWrite(6,high_speed+40);
  analogWrite(5,0);
}

void sol_denge(){
  analogWrite(9,high_speed+40);
  analogWrite(10,0);
  analogWrite(6,med_speed);
  analogWrite(5,0);
}

void sola_don() {
  sag_Motor.ileri(125);
  delay(250);
  sol_Motor.geri();
  delay(250);
}

void saga_don() {
  sol_Motor.ileri(125);
  delay(250);
  sag_Motor.geri();
  delay(250);
}

void dur(){
    sol_Motor.dur();
    sag_Motor.dur();
}

void duz_git(){
    sol_Motor.ileri(170);
    sag_Motor.ileri(125);
}