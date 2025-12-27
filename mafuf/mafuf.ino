// Defining the pins
const int trigPin1 = 3;
const int echoPin1 = 13;
const int trigPin2 = 5;
const int echoPin2 = 9;
const int trigPin3 = 6;
const int echoPin3 = A0; // Using analog pin A0 as a digital pin
int irpin = 2;
char command; // For Bluetooth command

// Motor pins
const int motorPin1 = 4;n    
const int motorPin2 = 7;
const int motorPin3 = 8;
const int motorPin4 = 12;
const int motorSpeedA = 10; // ENA pin for Motor A
const int motorSpeedB = 11; // ENB pin for Motor B

// Defining variables
long duration1;
long duration2;
long duration3;
int distanceleft;
int distancefront;
int distanceright;
int a = 0;
int speed = 200; // Initial speed (range 0-255)

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(irpin, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorSpeedA, OUTPUT);
  pinMode(motorSpeedB, OUTPUT);
}

void loop() {
  // Ultrasonic sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distanceleft = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.println(distanceleft);

  // Ultrasonic sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distancefront = duration2 * 0.034 / 2;
  Serial.print("Distance2: ");
  Serial.println(distancefront);

  // Ultrasonic sensor 3
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distanceright = duration3 * 0.034 / 2;
  Serial.print("Distance3: ");
  Serial.println(distanceright);

  // Read IR sensor value
  int s = digitalRead(irpin);

  // Bluetooth control
  if (Serial.available() > 0) {
    command = Serial.read();
    controlRobot(command);
  }

  // Autonomous control
  if (s == HIGH) { 
    // Detected hole or surface, stop
    stopRobot();
    a = 1;
  } else if ((a == 0) && (distanceleft <= 15 && distancefront > 15 && distanceright <= 15) || (a == 0) && (distanceleft > 15 && distancefront > 15 && distanceright > 15)) {
    // Move forward 
    moveForward();
  } else if ((a == 1) && (distanceleft <= 15 && distancefront <= 15 && distanceright > 15) || (distanceleft <= 15 && distancefront > 15 && distanceright > 15)) {
    // Turn left
    turnLeft();
    a = 0;
  } else if ((distanceleft > 15 && distancefront <= 15 && distanceright <= 15) || (distanceleft > 15 && distancefront > 15 && distanceright <= 15) || (distanceleft > 15 && distancefront <= 15 && distanceright > 15)) {
    // Turn right
    turnRight();
  }
}

void moveForward() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void turnLeft() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(100);
}

void turnRight() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stopRobot() {
  analogWrite(motorSpeedA, 0);
  analogWrite(motorSpeedB, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void controlRobot(char command) {
  switch (command) {
    case 'F':
      moveForward();
      break;
    case 'L':
      turnLeft();
      break;
    case 'R':
      turnRight();
      break;
    case 'S':
      stopRobot();
      break;
    case '1':
      speed = 100; // Low speed
      break;
    case '2':
      speed = 150; // Medium speed
      break;
    case '3':
      speed = 200; // High speed
      break;
    case '4':
      speed = 255; // Max speed
      break;
    default:
      stopRobot();
      break;
  }
}
