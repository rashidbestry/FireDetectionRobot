#include <Arduino.h>

// Motor pinleri
const int SAG_MOTOR_PIN1 = 10;
const int SAG_MOTOR_PIN2 = 9;
const int SOL_MOTOR_PIN1 = 5;
const int SOL_MOTOR_PIN2 = 6;

// Çizgi Sensor pinleri
const int LEFT_SENSOR_PIN = 2;
const int MED_SENSOR_PIN = 3;
const int RIGHT_SENSOR_PIN = 4;
// dönme sensörleri
const int TURN_LEFT_SENSOR_PIN = 12;
const int TURN_RIGHT_SENSOR_PIN = 11;
// Alev Sensor pin2
const int FIRE_PIN = 13;

// Hızlar

const int SLOW_SPEED = 50;
const int MED_SPEED = 100;
const int HIGH_SPEED = 200;

// Fan
const int FAN_INA = 7;
const int FAN_INB = 8;

// Front distance
constexpr int FRONT_TRIG_PIN = A4;
constexpr int FRONT_ECHO_PIN = A5;

long front_duration;
int front_distance;


//Back distance
const int BACK_TRIG_PIN = A0;
const int BACK_ECHO_PIN = A1;

long back_duration;
int back_distance;


const int BLACK = 1;
const int WHITE = 0;





// ---------------- MOTOR FUNCTIONS ----------------

// Motor ileri
void motor_ileri(int pin1, int pin2, int speed) {
  analogWrite(pin1, 0);
  analogWrite(pin2, speed);
}

// Motor geri
void motor_geri(int pin1, int pin2, int speed) {
  analogWrite(pin2, 0);
  analogWrite(pin1, speed);
}

// Motor dur
void motor_dur(int pin1, int pin2) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

// ---------------- ROBOT FUNCTIONS ----------------

void dur() {
  motor_dur(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2);
  motor_dur(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2);
}

void duz_git() {
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED+20);
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED);
}

void sag_denge() {
  analogWrite(SAG_MOTOR_PIN2, MED_SPEED);
  analogWrite(SAG_MOTOR_PIN1, 0);

  analogWrite(SOL_MOTOR_PIN2, HIGH_SPEED-50);
  analogWrite(SOL_MOTOR_PIN1, 0);
}

void sol_denge() {
  analogWrite(SAG_MOTOR_PIN2, HIGH_SPEED-50);
  analogWrite(SAG_MOTOR_PIN1, 0);

  analogWrite(SOL_MOTOR_PIN2, MED_SPEED);
  analogWrite(SOL_MOTOR_PIN1, 0);
}

void sola_don() {
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED-30);
  delay(250);
  motor_dur(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2);
  delay(250);
}

void saga_don() {
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED);
  delay(250);
  motor_dur(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2);
  delay(250);
}

int front_distance_value() {
  digitalWrite(FRONT_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(FRONT_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(FRONT_TRIG_PIN, LOW);
  front_duration = pulseIn(FRONT_ECHO_PIN, HIGH);
  front_distance = front_duration * 0.034 / 2;
  // Serial.print("front_distance: ");
  // Serial.print(front_distance);
  // Serial.println(" cm");
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
  // Serial.print("back_distance: ");
  // Serial.print(back_distance);
  // Serial.println(" cm");
  return back_distance;
}

// ---------------- FIRE DETECT ----------------

void fire_detect() {

  if (digitalRead(FIRE_PIN) == 0) {
    dur();
    digitalWrite(FAN_INA, LOW);
    digitalWrite(FAN_INB, HIGH);
  }
  if (digitalRead(FIRE_PIN) == 1) {
    digitalWrite(FAN_INA, LOW);
    digitalWrite(FAN_INB, LOW);
  }
}

// ---------------- LINE TRACK ----------------

void line_track() {


}

void setup() {
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

  pinMode(FIRE_PIN, INPUT);

  pinMode(FAN_INA, OUTPUT);
  pinMode(FAN_INB, OUTPUT);

  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);

  pinMode(BACK_TRIG_PIN, OUTPUT);
  pinMode(BACK_ECHO_PIN, INPUT);
}

int get_filtered_front_distance() {
  int sum = 0;
  for(int i = 0; i < 3; i++) {
    sum += front_distance_value();
    delay(15); // Short 15ms pause to prevent ultrasonic soundwaves from overlapping
  }
  return sum / 3; // Return the average
}

// Helper function for the back sensor
int get_filtered_back_distance() {
  int sum = 0;
  for(int i = 0; i < 3; i++) {
    sum += back_distance_value();
    delay(15);
  }
  return sum / 3;
}
enum RobotState {
  CIZGI_IZLE,
  KAVSAK_KARARI,
  ODAYA_GIRIS,
  ODA_TARA,
  ODADAN_CIKIS,
  YANGIN_SONDUR
};
RobotState currentState = CIZGI_IZLE;
int kavsakAsamasi=0;

// Kavşak Karar Algoritması (İş Mantığı)
void kavsak_karari_ver(int TLS, int TRS) {

  // MERKEZLEME: Tekerlekleri dönüş eksenine oturt
  // HAFIZALI KARAR AĞACI
  if (TLS == BLACK && TRS == BLACK) {
    // ÇİFT YÖNLÜ KAVŞAK (T veya + Şeklinde)
    if (kavsakAsamasi == 0) {
      saga_don();
      kavsakAsamasi = 1;
      currentState = ODAYA_GIRIS;
    }
    else if (kavsakAsamasi == 1) {
      duz_git();
      delay(300); // Karşı odaya geçiş
      dur();
      kavsakAsamasi = 2;
      currentState = ODAYA_GIRIS;
    }
    else if (kavsakAsamasi == 2) {
      saga_don();
      kavsakAsamasi = 0; // Hafızayı sıfırla
      currentState = CIZGI_IZLE;
    }
  }
  // SADECE SAĞA DALLANMA VARSA
  else if (TRS == BLACK && TLS == WHITE) {
    saga_don();
    currentState = ODAYA_GIRIS;
  }
  // SADECE SOLA DALLANMA VARSA
  else if (TLS == BLACK && TRS == WHITE) {
    sola_don();
    currentState = ODAYA_GIRIS;
  }
}

void loop() {
  // 1. ACİL DURUM
  int LS = digitalRead(LEFT_SENSOR_PIN);
  int MS = digitalRead(MED_SENSOR_PIN);
  int RS = digitalRead(RIGHT_SENSOR_PIN);
  int TRS  = digitalRead(TURN_LEFT_SENSOR_PIN);
  int TLS  = digitalRead(TURN_RIGHT_SENSOR_PIN);

  if (digitalRead(FIRE_PIN) == 0) {
    currentState = YANGIN_SONDUR;
  }

  // 2. SENSÖR OKUMALARI
  int avg_front_dist = get_filtered_front_distance();

  // 3. DURUM MAKİNESİ
  switch (currentState) {

    case CIZGI_IZLE:
      if (TLS == BLACK || TRS == BLACK) {
        currentState = KAVSAK_KARARI;
      } else {

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
      break;

    case KAVSAK_KARARI:
      kavsak_karari_ver(TLS, TRS);
      break;

    case ODAYA_GIRIS:
      line_track();
      if (avg_front_dist <= 15) {
        dur();
        currentState = ODA_TARA;
      }
      break;

    case ODA_TARA:
      saga_don();
      currentState = ODADAN_CIKIS;
      break;

    case ODADAN_CIKIS:
      saga_don();
      saga_don();
      currentState = CIZGI_IZLE;
      break;

    case YANGIN_SONDUR:
      fire_detect();
      break;
  }

}

