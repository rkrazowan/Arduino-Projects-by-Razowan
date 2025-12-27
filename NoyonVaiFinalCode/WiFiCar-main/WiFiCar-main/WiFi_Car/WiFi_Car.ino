// Motor pins
const int motorPin1 = 30; // Motor A IN1
const int motorPin2 = 29; // Motor A IN2
const int motorPin3 = 31; // Motor B IN3
const int motorPin4 = 32; // Motor B IN4
const int motorSpeedA = 10; // ENA pin for Motor A (PWM)
const int motorSpeedB = 11; // ENB pin for Motor B (PWM)

// Speed setting
int speed = 200; // Initial speed (range 0-255)

// Bluetooth command variable
char command;

void setup() {
  Serial.begin(9600); // Initialize serial communication with Bluetooth module

  // Initialize motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorSpeedA, OUTPUT);
  pinMode(motorSpeedB, OUTPUT);

  // Print initial message
  Serial.println("Car Control Initialized. Awaiting commands...");
}

void loop() {
  // Check if data is available from Bluetooth module
  if (Serial.available() > 0) {
    command = Serial.read();
    Serial.print("Received command: ");
    Serial.println(command);

    // Process Bluetooth commands
    switch (command) {
      case 'F': // Move forward
        moveForward();
        Serial.println("Moving Forward");
        break;
      case 'B': // Move backward
        moveBackward();
        Serial.println("Moving Backward");
        break;
      case 'L': // Turn left
        turnLeft();
        Serial.println("Turning Left");
        break;
      case 'R': // Turn right
        turnRight();
        Serial.println("Turning Right");
        break;
      case 'S': // Stop
        stopCar();
        Serial.println("Stopping");
        break;
      case '1': // Set speed low
        speed = 100;
        Serial.println("Speed set to Low");
        break;
      case '2': // Set speed medium
        speed = 150;
        Serial.println("Speed set to Medium");
        break;
      case '3': // Set speed high
        speed = 200;
        Serial.println("Speed set to High");
        break;
      case '4': // Set speed max
        speed = 255;
        Serial.println("Speed set to Max");
        break;
      default:
        stopCar();
        Serial.println("Unknown Command. Stopping");
        break;
    }
  }

  delay(100); // Small delay for stability
}

// Function to move the car forward
void moveForward() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Function to move the car backward
void moveBackward() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

// Function to turn the car left
void turnLeft() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Function to turn the car right
void turnRight() {
  analogWrite(motorSpeedA, speed);
  analogWrite(motorSpeedB, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

// Function to stop the car
void stopCar() {
  analogWrite(motorSpeedA, 0);
  analogWrite(motorSpeedB, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
