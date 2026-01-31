#include <Arduino.h>

const int SUCTION_PIN = 7;
const int BRUSH_PIN = 8;

const int DEFAULT_SPEED = 200;
const int MIN_DISTANCE = 5;
const int EDGE_DISTANCE = 50;

const unsigned long SENSOR_INTERVAL = 120;
const unsigned long REVERSE_DURATION = 700;
const unsigned long TURN_DURATION = 650;
const unsigned long TELEMETRY_INTERVAL = 500;

class Motor {
  int pinFwd, pinBwd, pwmPin;
public:
  Motor(int f = -1, int b = -1, int pwm = -1): pinFwd(f), pinBwd(b), pwmPin(pwm) {}
  void begin() {
    if (pinFwd != -1) pinMode(pinFwd, OUTPUT);
    if (pinBwd != -1) pinMode(pinBwd, OUTPUT);
    if (pwmPin != -1) pinMode(pwmPin, OUTPUT);
    stop();
  }
  void drive(int speed) {
    if (speed == 0) { stop(); return; }
    if (speed > 0) {
      digitalWrite(pinFwd, HIGH);
      digitalWrite(pinBwd, LOW);
      if (pwmPin != -1) analogWrite(pwmPin, constrain(speed, 0, 255));
    } else {
      digitalWrite(pinFwd, LOW);
      digitalWrite(pinBwd, HIGH);
      if (pwmPin != -1) analogWrite(pwmPin, constrain(-speed, 0, 255));
    }
  }
  void stop() {
    digitalWrite(pinFwd, LOW);
    digitalWrite(pinBwd, LOW);
    if (pwmPin != -1) analogWrite(pwmPin, 0);
  }
};

class Ultrasonic {
  int trigPin, echoPin;
public:
  Ultrasonic(int t = -1, int e = -1): trigPin(t), echoPin(e) {}
  void begin() {
    if (trigPin != -1) pinMode(trigPin, OUTPUT);
    if (echoPin != -1) pinMode(echoPin, INPUT);
    if (trigPin != -1) digitalWrite(trigPin, LOW);
  }
  int readCM() {
    if (trigPin == -1 || echoPin == -1) return 1000;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH, 20000);
    if (duration == 0) return 1000;
    return int(duration * 0.034 / 2);
  }
};

Motor leftMotor(3, 4);
Motor rightMotor(5, 6);

Ultrasonic sensorFront(9, 10);
Ultrasonic sensorBack(11, 12);
Ultrasonic sensorLeft(13, A0);
Ultrasonic sensorRight(A1, A2);

enum SensorIndex { S_FRONT = 0, S_BACK = 1, S_LEFT = 2, S_RIGHT = 3 };
enum RobotState { STATE_CLEANING, STATE_REVERSING, STATE_TURNING, STATE_STOPPED };

RobotState state = STATE_CLEANING;
unsigned long stateStart = 0;
unsigned long lastSensorMillis = 0;
unsigned long lastTelemetryMillis = 0;

int distances[4] = {1000, 1000, 1000, 1000};

void driveBoth(int speed) {
  leftMotor.drive(speed);
  rightMotor.drive(speed);
}
void stopBoth() {
  leftMotor.stop();
  rightMotor.stop();
}
void pivotTurnLeft(int speed) {
  leftMotor.drive(speed);
  rightMotor.stop();
}
void pivotTurnRight(int speed) {
  rightMotor.drive(speed);
  leftMotor.stop();
}

int preferTurnDirection() {
  if (distances[S_LEFT] < distances[S_RIGHT]) return +1;
  if (distances[S_RIGHT] < distances[S_LEFT]) return -1;
  return +1;
}

void readSensors() {
  distances[S_FRONT] = sensorFront.readCM();
  distances[S_BACK]  = sensorBack.readCM();
  distances[S_LEFT]  = sensorLeft.readCM();
  distances[S_RIGHT] = sensorRight.readCM();
}

void enterState(RobotState s) {
  state = s;
  stateStart = millis();
  switch (s) {
    case STATE_CLEANING: driveBoth(DEFAULT_SPEED); break;
    case STATE_REVERSING: driveBoth(-DEFAULT_SPEED); break;
    case STATE_TURNING: break;
    case STATE_STOPPED: stopBoth(); break;
  }
}

void setup() {
  Serial.begin(9600);
  leftMotor.begin();
  rightMotor.begin();
  sensorFront.begin();
  sensorBack.begin();
  sensorLeft.begin();
  sensorRight.begin();
  pinMode(SUCTION_PIN, OUTPUT);
  pinMode(BRUSH_PIN, OUTPUT);
  digitalWrite(SUCTION_PIN, HIGH);
  digitalWrite(BRUSH_PIN, HIGH);
  enterState(STATE_CLEANING);
}

void evaluateEnvironment() {
  if (distances[S_FRONT] >= EDGE_DISTANCE) { enterState(STATE_REVERSING); return; }
  if (distances[S_FRONT] <= MIN_DISTANCE) { enterState(STATE_REVERSING); return; }
  if (distances[S_LEFT] >= EDGE_DISTANCE) { enterState(STATE_REVERSING); return; }
  if (distances[S_RIGHT] >= EDGE_DISTANCE) { enterState(STATE_REVERSING); return; }
  if (state != STATE_CLEANING) enterState(STATE_CLEANING);
}

void loop() {
  unsigned long now = millis();

  if (now - lastSensorMillis >= SENSOR_INTERVAL) {
    readSensors();
    lastSensorMillis = now;
    evaluateEnvironment();
  }

  if (state == STATE_REVERSING) {
    if (now - stateStart >= REVERSE_DURATION) {
      int dir = preferTurnDirection();
      enterState(STATE_TURNING);
      if (dir > 0) pivotTurnRight(DEFAULT_SPEED);
      else pivotTurnLeft(DEFAULT_SPEED);
    }
  }
  else if (state == STATE_TURNING) {
    if (now - stateStart >= TURN_DURATION) enterState(STATE_CLEANING);
  }
  else if (state == STATE_CLEANING) {
    driveBoth(DEFAULT_SPEED);
  }

  if (now - lastTelemetryMillis >= TELEMETRY_INTERVAL) {
    lastTelemetryMillis = now;
    Serial.print("State: ");
    switch (state) {
      case STATE_CLEANING: Serial.print("CLEAN "); break;
      case STATE_REVERSING: Serial.print("REV "); break;
      case STATE_TURNING: Serial.print("TURN "); break;
      case STATE_STOPPED: Serial.print("STOP "); break;
    }
    Serial.print("F:"); Serial.print(distances[S_FRONT]);
    Serial.print(" B:"); Serial.print(distances[S_BACK]);
    Serial.print(" L:"); Serial.print(distances[S_LEFT]);
    Serial.print(" R:"); Serial.println(distances[S_RIGHT]);
  }

  delay(2);
}
