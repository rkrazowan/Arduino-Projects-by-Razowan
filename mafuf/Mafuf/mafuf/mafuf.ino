// Defining the pins for Ultrasonic Sensors
const int trigPin1 = 22; // Updated for Arduino Mega
const int echoPin1 = 23; // Updated for Arduino Mega
const int trigPin2 = 24; // Updated for Arduino Mega
const int echoPin2 = 25; // Updated for Arduino Mega
const int trigPin3 = 26; // Updated for Arduino Mega
const int echoPin3 = 27; // Updated for Arduino Mega

// Defining the IR sensor pin
int irpin = 28; // Updated for Arduino Mega
char command; // For Bluetooth command

// Motor pins
const int motorPin1 = 29; // Updated for Arduino Mega
const int motorPin2 = 30; // Updated for Arduino Mega
const int motorPin3 = 31; // Updated for Arduino Mega
const int motorPin4 = 32; // Updated for Arduino Mega
const int motorSpeedA = 10; // Updated for Arduino Mega
const int motorSpeedB = 11; // Updated for Arduino Mega

// Defining variables for sensor readings
long duration1, duration2, duration3;
int distanceleft, distancefront, distanceright;
int a = 0;
int speed = 200; // Initial speed (range 0-255)
bool manualMode = false; // Flag for mode selection

void setup() {
  Serial.begin(9600); // Initialize serial communication

  // Initialize ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);

  // Initialize IR sensor and motor control pins
  pinMode(irpin, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorSpeedA, OUTPUT);
  pinMode(motorSpeedB, OUTPUT);
}

void loop() {
  // Check for Bluetooth commands
  if (Serial.available() > 0) {
    command = Serial.read();
    if (command == 'V') {
      manualMode = true; // Enable manual mode
      Serial.println("Manual Mode");
    } else if (command == 'v') {
      manualMode = false; // Enable autonomous mode
      Serial.println("Autonomous Mode");
    } else if (manualMode) {
      controlRobot(command);
      return; // Skip autonomous control while in manual mode
    }
  }

  // Autonomous control logic
  if (!manualMode) {
    // Read distance from ultrasonic sensor 1 (left)
    distanceleft = getDistance(trigPin1, echoPin1);
    Serial.print("Distance Left: ");
    Serial.println(distanceleft);

    // Read distance from ultrasonic sensor 2 (front)
    distancefront = getDistance(trigPin2, echoPin2);
    Serial.print("Distance Front: ");
    Serial.println(distancefront);

    // Read distance from ultrasonic sensor 3 (right)
    distanceright = getDistance(trigPin3, echoPin3);
    Serial.print("Distance Right: ");
    Serial.println(distanceright);

    // Read IR sensor value
    int s = digitalRead(irpin);

    if (s == HIGH) {
      // Detected hole or edge, stop the robot
      stopRobot();
      a = 1;
    } else if ((a == 0) && (distanceleft <= 15 && distancefront > 15 && distanceright <= 15) || 
               (a == 0) && (distanceleft > 15 && distancefront > 15 && distanceright > 15)) {
      // Move forward
      moveForward();
    } else if ((a == 1) && (distanceleft <= 15 && distancefront <= 15 && distanceright > 15) || 
               (distanceleft <= 15 && distancefront > 15 && distanceright > 15)) {
      // Turn left
      turnLeft();
      a = 0;
    } else if ((distanceleft > 15 && distancefront <= 15 && distanceright <= 15) || 
               (distanceleft > 15 && distancefront > 15 && distanceright <= 15) || 
               (distanceleft > 15 && distancefront <= 15 && distanceright > 15)) {
      // Turn right
      turnRight();
    }
  }

  delay(100); // Small delay to stabilize readings
}

// Function to get distance from ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

// Function to move the robot forward
void moveForward() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Function to turn the robot left
void turnLeft() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(100);
}

// Function to turn the robot right
void turnRight() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Function to stop the robot
void stopRobot() {
  analogWrite(motorSpeedA, 0);
  analogWrite(motorSpeedB, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

// Function to control the robot via Bluetooth
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
