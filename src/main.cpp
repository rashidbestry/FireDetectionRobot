#include <Arduino.h>

// --- MOTOR PİNLERİ ---
const int SAG_MOTOR_PIN1 = 10;
const int SAG_MOTOR_PIN2 = 9;
const int SOL_MOTOR_PIN1 = 5;
const int SOL_MOTOR_PIN2 = 6;

// --- MERKEZ 3 SENSÖR (DENGE İÇİN) ---
const int LEFT_SENSOR_PIN = 2;
const int MED_SENSOR_PIN = 3;
const int RIGHT_SENSOR_PIN = 4;

// --- YENİ 2 DIŞ SENSÖR (KAVŞAK VE DÖNÜŞ İÇİN) ---
const int EXT_LEFT_SENSOR_PIN = 12;
const int EXT_RIGHT_SENSOR_PIN = 11;

// --- DİĞER PİNLER ---
const int FIRE_PIN = 13;
const int FAN_INA = 7;
const int FAN_INB = 8;
const int BLACK = 1;
const int WHITE = 0;

const int MED_SPEED = 125;
const int HIGH_SPEED = 255;
const int LOW_SPEED = 55;

// --- HAFIZA ---
int kavsakAsamasi = 0;

// --- DURUM MAKİNESİ (Sadece 2 durum kaldı!) ---
enum RobotState {
  CIZGI_IZLE,
  YANGIN_SONDUR
};
RobotState currentState = CIZGI_IZLE;

// ---------------- MOTOR FONKSİYONLARI ----------------

void motor_ileri(int pin1, int pin2, int speed) { analogWrite(pin1, 0); analogWrite(pin2, speed); }
void motor_geri(int pin1, int pin2, int speed) { analogWrite(pin2, 0); analogWrite(pin1, speed); }
void motor_dur(int pin1, int pin2) { digitalWrite(pin1, LOW); digitalWrite(pin2, LOW); }

void dur() {
  motor_dur(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2);
  motor_dur(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2);
}

void duz_git() {
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED + 40);
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED);
}

// ---------------- SENSÖR KONTROLLÜ DÖNÜŞLER ----------------

void sola_don_90() {
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, HIGH_SPEED);
  motor_geri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, LOW_SPEED);
  //motor_dur(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2);
  delay(250);

  // while(digitalRead(MED_SENSOR_PIN) == WHITE) {
  //   motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED);
  //   motor_geri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED);
  // }
  // dur();
}

void saga_don_90() {
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
  motor_geri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, LOW_SPEED);
  //motor_dur(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2);
  delay(250);


  // while(digitalRead(MED_SENSOR_PIN) == WHITE) {
  //   motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED);
  //   motor_geri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED);
  // }
  // dur();
}

void tam_tur_don() {
  dur();
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
  motor_geri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2,HIGH_SPEED );
  delay(600);

  // while(digitalRead(MED_SENSOR_PIN) == WHITE) {
  //   motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED);
  //   motor_geri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED);
  // }

}

// ---------------- ÇİZGİ TAKİP YARDIMCISI ----------------
void cizgiyi_takip_et(int LS, int MS, int RS) {
  if (LS == BLACK && MS == WHITE && RS == WHITE) {
      motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, HIGH_SPEED);
      motor_dur(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2);
  }
  else if (LS == WHITE && MS == WHITE && RS == BLACK) {
      motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
      motor_dur(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2);
  }
  else {
      duz_git();
  }
}

// ---------------- KAVŞAK KARAR MANTIĞI ----------------
void kavsak_karari_ver(int EXT_L, int EXT_R) {
  dur();

  // ÇİFT YÖNLÜ KAVŞAK
  if (EXT_L == BLACK && EXT_R == BLACK) {
    if (kavsakAsamasi == 0) {
      saga_don_90();
      kavsakAsamasi = 1;
    }
    else if (kavsakAsamasi == 1) {
      duz_git();
      delay(300);
      dur();
      kavsakAsamasi = 2;
    }
    else if (kavsakAsamasi == 2) {
      saga_don_90();
      kavsakAsamasi = 0;
    }
  }

}

// ---------------- KURULUM VE ANA DÖNGÜ ----------------

void setup() {
  pinMode(SAG_MOTOR_PIN1, OUTPUT); pinMode(SAG_MOTOR_PIN2, OUTPUT);
  pinMode(SOL_MOTOR_PIN1, OUTPUT); pinMode(SOL_MOTOR_PIN2, OUTPUT);
  pinMode(LEFT_SENSOR_PIN, INPUT); pinMode(MED_SENSOR_PIN, INPUT); pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(EXT_LEFT_SENSOR_PIN, INPUT); pinMode(EXT_RIGHT_SENSOR_PIN, INPUT);
  pinMode(FIRE_PIN, INPUT); pinMode(FAN_INA, OUTPUT); pinMode(FAN_INB, OUTPUT);
}

void loop() {

  // 1. ACİL DURUM KONTROLÜ
   if (digitalRead(FIRE_PIN) == 0) {
     currentState = YANGIN_SONDUR;
   }

   int LS = digitalRead(LEFT_SENSOR_PIN);
   int MS = digitalRead(MED_SENSOR_PIN);
   int RS = digitalRead(RIGHT_SENSOR_PIN);
   int EXT_L = digitalRead(EXT_LEFT_SENSOR_PIN);
   int EXT_R = digitalRead(EXT_RIGHT_SENSOR_PIN);

   // 2. DURUM MAKİNESİ
   switch (currentState) {

     case CIZGI_IZLE:
       // A. KAVŞAK GELDİ Mİ?
       if (EXT_L == BLACK && EXT_R == WHITE) {
         sola_don_90();

       }
       else if (EXT_L == WHITE && EXT_R == BLACK) {
         saga_don_90();
       }
       else if (EXT_L == BLACK && EXT_R == BLACK) {
         kavsak_karari_ver(EXT_L, EXT_R);
       }
       // if (EXT_L == BLACK || EXT_R == BLACK) {
       //   dur();
       //   kavsak_karari_ver(EXT_L, EXT_R);
       // }
       // B. ÇİZGİ BİTTİ Mİ? (Odanın Sonu / Dead End)
       // else if (LS == WHITE && MS == WHITE && RS == WHITE) {
       //   motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
       //   // Odanın sonuna gelindi, ateş bulunamadı. Geri dön!
       //   //tam_tur_don();
       // }
       // C. NORMAL ÇİZGİ TAKİBİ

       else {
         cizgiyi_takip_et(LS, MS, RS);
       }
       break;

     case YANGIN_SONDUR:
       dur();
       digitalWrite(FAN_INA, LOW);
       digitalWrite(FAN_INB, HIGH);
       while(1);
       break;
  }
}
