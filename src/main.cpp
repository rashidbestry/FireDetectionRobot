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

// Alev Sensor pin
const int FIRE_PIN = 13;

// Hızlar

const int SLOW_SPEED = 50;
const int MED_SPEED = 125;
const int HIGH_SPEED = 255;

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
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, MED_SPEED+40);
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, MED_SPEED);
}

void sag_denge() {
  analogWrite(SAG_MOTOR_PIN2, MED_SPEED);
  analogWrite(SAG_MOTOR_PIN1, 0);

  analogWrite(SOL_MOTOR_PIN2, HIGH_SPEED);
  analogWrite(SOL_MOTOR_PIN1, 0);
}

void sol_denge() {
  analogWrite(SAG_MOTOR_PIN2, HIGH_SPEED);
  analogWrite(SAG_MOTOR_PIN1, 0);

  analogWrite(SOL_MOTOR_PIN2, MED_SPEED);
  analogWrite(SOL_MOTOR_PIN1, 0);
}

void sola_don() {
  motor_ileri(SAG_MOTOR_PIN1, SAG_MOTOR_PIN2, HIGH_SPEED);
  delay(250);
  motor_dur(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2);
  delay(250);
}

void saga_don() {
  motor_ileri(SOL_MOTOR_PIN1, SOL_MOTOR_PIN2, HIGH_SPEED);
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

  int LS = digitalRead(LEFT_SENSOR_PIN);
  int MS = digitalRead(MED_SENSOR_PIN);
  int RS = digitalRead(RIGHT_SENSOR_PIN);

  if (LS == BLACK && MS == WHITE && RS == WHITE) {
    sol_denge();
  }
  else if (LS == WHITE && MS == WHITE && RS == BLACK) {
    sag_denge();
  }
  else if (LS == WHITE && MS == BLACK && RS == BLACK) {
    sag_denge();
  }
  else if (LS == BLACK && MS == BLACK && RS == WHITE) {
    sol_denge();
  }
  else {
    duz_git();
  }
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

  pinMode(FIRE_PIN, INPUT);

  pinMode(FAN_INA, OUTPUT);
  pinMode(FAN_INB, OUTPUT);

  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);

  pinMode(BACK_TRIG_PIN, OUTPUT);
  pinMode(BACK_ECHO_PIN, INPUT);
}

void loop() {


  while ((back_distance_value() + back_distance_value() + back_distance_value()) <= 30 ) {
    dur();
    delay(1000);
  }
  while ((front_distance_value() + front_distance_value() + front_distance_value()) <= 30 ) {
    dur();
    delay(1000);
  }

  fire_detect();
  line_track();
}
