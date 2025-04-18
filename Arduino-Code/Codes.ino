// ==================== MOTOR DEFINITIONS ====================
const int motorLeftFwd = 3;
const int motorLeftBwd = 4;
const int motorRightFwd = 5;
const int motorRightBwd = 6;

// ==================== SUCTION & BRUSH ====================
const int suctionMotor = 7;
const int brushMotor = 8;

// ==================== ULTRASONIC SENSOR PINS ====================
// Format: {TrigPin, EchoPin}
struct UltrasonicSensor {
  int trigPin;
  int echoPin;
};

// Assign sensors
UltrasonicSensor sensorFront = {9, 10};
UltrasonicSensor sensorBack  = {11, 12};
UltrasonicSensor sensorLeft  = {13, A0};
UltrasonicSensor sensorRight = {A1, A2};

// ==================== DISTANCE THRESHOLDS ====================
const int minDistance = 5;       // Obstacle too close
const int maxSafeDistance = 50;  // Potential edge

// ==================== SETUP ====================
void setup() {
  // Motor pins
  pinMode(motorLeftFwd, OUTPUT);
  pinMode(motorLeftBwd, OUTPUT);
  pinMode(motorRightFwd, OUTPUT);
  pinMode(motorRightBwd, OUTPUT);

  pinMode(suctionMotor, OUTPUT);
  pinMode(brushMotor, OUTPUT);

  // Sensor pins
  setupUltrasonic(sensorFront);
  setupUltrasonic(sensorBack);
  setupUltrasonic(sensorLeft);
  setupUltrasonic(sensorRight);

  // Start suction and cleaning
  digitalWrite(suctionMotor, HIGH);
  digitalWrite(brushMotor, HIGH);

  Serial.begin(9600);
}

void loop() {
  // === Read all sensor distances ===
  int distFront = getDistance(sensorFront);
  int distBack = getDistance(sensorBack);
  int distLeft = getDistance(sensorLeft);
  int distRight = getDistance(sensorRight);

  // === DEBUG ===
  Serial.print("F:"); Serial.print(distFront);
  Serial.print(" B:"); Serial.print(distBack);
  Serial.print(" L:"); Serial.print(distLeft);
  Serial.print(" R:"); Serial.println(distRight);

  // === Movement Decision Logic ===
  if (distFront >= maxSafeDistance) {
    // Edge ahead
    stopMovement();
    delay(500);
    reverse();
    delay(1000);
    if (distLeft < distRight) {
      turnRightSimple();
    } else {
      turnLeftSimple();
    }
    delay(800);
  }
  else if (distFront <= minDistance) {
    // Obstacle ahead
    stopMovement();
    delay(500);
    reverse();
    delay(1000);
    turnLeftSimple();
    delay(800);
  }
  else if (distLeft >= maxSafeDistance) {
    // Edge on left
    stopMovement();
    delay(500);
    turnRightSimple();
    delay(800);
  }
  else if (distRight >= maxSafeDistance) {
    // Edge on right
    stopMovement();
    delay(500);
    turnLeftSimple();
    delay(800);
  }
  else {
    moveForward();
  }

  delay(100);
}

// ==================== MOVEMENT FUNCTIONS ====================

void moveForward() {
  digitalWrite(motorLeftFwd, HIGH);
  digitalWrite(motorLeftBwd, LOW);
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorRightBwd, LOW);
}

void reverse() {
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBwd, HIGH);
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBwd, HIGH);
}

void stopMovement() {
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBwd, LOW);
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBwd, LOW);
}

void turnRightSimple() {
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBwd, LOW);
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorRightBwd, LOW);
}

void turnLeftSimple() {
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBwd, LOW);
  digitalWrite(motorLeftFwd, HIGH);
  digitalWrite(motorLeftBwd, LOW);
}

// ==================== SENSOR FUNCTIONS ====================

void setupUltrasonic(UltrasonicSensor s) {
  pinMode(s.trigPin, OUTPUT);
  pinMode(s.echoPin, INPUT);
}

int getDistance(UltrasonicSensor s) {
  digitalWrite(s.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(s.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(s.trigPin, LOW);

  long duration = pulseIn(s.echoPin, HIGH, 20000);  // timeout in 20 ms
  if (duration == 0) return 1000; // no echo
  int distance = duration * 0.034 / 2; // convert to cm
  return distance;
}
