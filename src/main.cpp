#include <Arduino.h>

// -------------------------------- MOTOR PİNLERİ --------------------------------
const int SOL_MOTOR_PIN1 = 5;
const int SOL_MOTOR_PIN2 = 6;
const int SAG_MOTOR_PIN2 = 9;
const int SAG_MOTOR_PIN1 = 10;

// ------------------------ MERKEZ 3 SENSÖR (DENGE İÇİN) -------------------------
const int LEFT_SENSOR_PIN = 2;
const int MED_SENSOR_PIN = 3;
const int RIGHT_SENSOR_PIN = 4;

// ------------------- YENİ 2 DIŞ SENSÖR (KAVŞAK VE DÖNÜŞ İÇİN) ------------------
const int TURN_RIGHT_SENSOR_PIN = 11;
const int TURN_LEFT_SENSOR_PIN = 12;

// ---------------------------- MESAFE SENSÖR PİNLERİ ----------------------------
const int FRONT_TRIG_PIN = A4;
const int FRONT_ECHO_PIN = A5;
const int BACK_TRIG_PIN = A0;
const int BACK_ECHO_PIN = A1;

float front_duration;
float back_duration;
int front_distance;
int back_distance;

// --------------------------------- DİĞER PİNLER --------------------------------
const int FIRE_PIN = 13;
const int FAN_INA = 7;
const int FAN_INB = 8;

// ------------------------------ TEMEL DEĞİŞKENLER ------------------------------

const int BLACK = 1;
const int WHITE = 0;

const int LOW_SPEED = 55;
const int MED_SPEED = 125;
const int HIGH_SPEED = 255;

// --- HAFIZA ---
String stack[10];
int stack_sayac = 0;

// --- DURUM MAKİNESİ ---
enum RobotState {CIZGI_IZLE, YANGIN_SONDUR, DUVAR, ODAYA_GIRIS};
RobotState currentState = CIZGI_IZLE;

// ----------------------------- MOTOR FONKSİYONLARI -----------------------------
void motor_ileri(int pin1, int pin2, int speed) {
  analogWrite(pin1, 0); analogWrite(pin2, speed);
}
void motor_geri(int pin1, int pin2, int speed) {
  analogWrite(pin2, 0); analogWrite(pin1, speed);
}
void motor_dur(int pin1, int pin2) {
  digitalWrite(pin1, LOW); digitalWrite(pin2, LOW);
}

// ----------------------------- ROBOT FONKSİYONLARI -----------------------------
void duz_git() {
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED + 40);
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED);
}

void geri_git() {
  motor_geri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED);
  motor_geri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED + 40);
}

void dur() {
  motor_dur(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2);
  motor_dur(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2);
}

void sola_don_90() {
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, HIGH_SPEED);
  motor_geri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, LOW_SPEED);
  delay(250);
}

void saga_don_90() {
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
  motor_geri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, LOW_SPEED);
  delay(250);
}

void tam_tur_don() {
  dur();
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
  motor_geri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, HIGH_SPEED);
  delay(550);
}

void cizgiyi_takip_et(int LS, int MS, int RS) {
  if (LS == BLACK && MS == WHITE && RS == WHITE) {
      motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, HIGH_SPEED);
      motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, LOW_SPEED);
  }
  else if (LS == WHITE && MS == WHITE && RS == BLACK) {
      motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
      motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, LOW_SPEED);
  }
  else {
      duz_git();
  }
}

// ----------------------------- KAVŞAK KARAR MANTIĞI ----------------------------
void kavsak_karari_ver(int TURN_L, int TURN_R) {
    // Hafızalı Karar Ağacı (SOL -> SAĞ -> İLERİ Önceliği)
    if (TURN_L == BLACK && TURN_R == BLACK && stack->length() == 0) {
        stack[0] = "DUZ";
        stack_sayac++;
        sola_don_90();
        currentState = ODAYA_GIRIS;
    }
    else if (TURN_L == BLACK && TURN_R == BLACK && stack->length() > 0) {
        stack[0] = "SAG";
        stack_sayac++;
        duz_git();
        delay(1000);
        currentState = ODAYA_GIRIS;
}
    }
    // SADECE SOLA DALLANMA VARSA
    else if (TURN_L == BLACK && TURN_R == WHITE) {
        sola_don_90();
        currentState = ODAYA_GIRIS;
    }
    // SADECE SAĞA DALLANMA VARSA
    else if (TURN_R == BLACK && TURN_L == WHITE) {
        saga_don_90();
        currentState = ODAYA_GIRIS;
    }else {
        cizgiyi_takip_et(0, 0, 0);
    }
}

// ------------------------------- MESAFE İŞLEMLERİ ------------------------------
int front_distance_value() {
    digitalWrite(FRONT_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(FRONT_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(FRONT_TRIG_PIN, LOW);
    front_duration = pulseIn(FRONT_ECHO_PIN, HIGH);
    front_distance = front_duration * 0.034 / 2;
    Serial.println(front_distance);
    return front_distance;
}

int back_distance_value() {
    digitalWrite(BACK_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(BACK_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(BACK_TRIG_PIN, LOW);
    back_duration = pulseIn(BACK_ECHO_PIN, HIGH);
    back_distance = back_duration * 0.034 / 2;
    return back_distance;
}

// ------------------------------------ SETUP ------------------------------------
void setup() {
  kavsakAsamasi = 0;
  Serial.begin(9600);

  pinMode(SAG_MOTOR_PIN1, OUTPUT);
  pinMode(SAG_MOTOR_PIN2, OUTPUT);
  pinMode(SOL_MOTOR_PIN1, OUTPUT);
  pinMode(SOL_MOTOR_PIN2, OUTPUT);

  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(MED_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
  pinMode(TURN_LEFT_SENSOR_PIN, INPUT);
  pinMode(TURN_RIGHT_SENSOR_PIN, INPUT);

  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);
  pinMode(BACK_TRIG_PIN, OUTPUT);
  pinMode(BACK_ECHO_PIN, INPUT);

  pinMode(FIRE_PIN, INPUT);
  pinMode(FAN_INA, OUTPUT);
  pinMode(FAN_INB, OUTPUT);
}

// ------------------------------------ LOOP -------------------------------------
void loop() {
   // 1. Duvar Kontrolü (Hata önleyici > 0 şartı eklendi)
   int mesafe = front_distance_value();
   if (mesafe > 0 && mesafe < 5) {
       currentState = DUVAR;
   }

   // 2. Acil Durum Kontrolü
   if (digitalRead(FIRE_PIN) == 0) {
     currentState = YANGIN_SONDUR;
   }

   // 3. Sensör Okumaları
   int LS = digitalRead(LEFT_SENSOR_PIN);
   int MS = digitalRead(MED_SENSOR_PIN);
   int RS = digitalRead(RIGHT_SENSOR_PIN);
   int TURN_L = digitalRead(TURN_LEFT_SENSOR_PIN);
   int TURN_R = digitalRead(TURN_RIGHT_SENSOR_PIN);

   // 4. Durum Makinesi
   switch (currentState) {

     case CIZGI_IZLE:
       // HATA DÜZELTİLDİ: && yerine || kullanıldı.
       if (TURN_L == BLACK || TURN_R == BLACK) {
         dur();
         kavsak_karari_ver(TURN_L, TURN_R);
       }
       else {
         cizgiyi_takip_et(LS, MS, RS);
       }
       break;

     case ODAYA_GIRIS:


       break;

     case DUVAR:
       dur();
       geri_git();
       delay(200);
       dur();
       delay(500);
       tam_tur_don();
       dur();
       delay(500);
       currentState = CIZGI_IZLE;
       break;

     case YANGIN_SONDUR:
       dur();
       digitalWrite(FAN_INA, LOW);
       digitalWrite(FAN_INB, HIGH);
       while(1); // Yangın sönene kadar sistemi kilitle
       break;
   }
}