#include <Servo.h> // Include the Servo library.

#define in1 8  // L298N Motor Driver pins.
#define in2 9
#define in3 10
#define in4 11
#define motorPin7 7 // Define the pin for the additional motor.
#define servoPin 6  // Define the pin for the servo motor.

int command; // Int to store app command state.
int Speed = 204; // 0 - 255.
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0; // Set the radius of a turn, 0 - 255. Note: the robot will malfunction if this is higher than int Speed.
int brakeTime = 45;
int brkonoff = 1; // 1 for the electronic braking system, 0 for normal.
boolean lightFront = false;
boolean lightBack = false;
boolean horn = false;
boolean servoActive = false; // Variable to track the servo's active state.

Servo myServo; // Create a Servo object.

unsigned long previousMillis = 0; // Store the last time the servo was updated.
const long interval = 15;         // Interval for updating the servo position.
int servoPos = 0;                 // Store the current servo position.
int servoIncrement = 1;           // Direction of servo movement (1 for increasing, -1 for decreasing).

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(motorPin7, OUTPUT); // Set the pin for the additional motor as an output.
  
  myServo.attach(servoPin); // Attach the servo to the defined pin.
  myServo.write(90); // Set the servo to the neutral position (90 degrees).
  
  Serial.begin(9600);  // Set the baud rate to your Bluetooth module.
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    Serial.println(command);
    Stop(); // Initialize with motors stopped.
    
    switch (command) {
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'G':
        forwardleft();
        break;
      case 'I':
        forwardright();
        break;
      case 'H':
        backleft();
        break;
      case 'J':
        backright();
        break;
      case '0':
        Speed = 100;
        break;
      case '1':
        Speed = 140;
        break;
      case '2':
        Speed = 153;
        break;
      case '3':
        Speed = 165;
        break;
      case '4':
        Speed = 178;
        break;
      case '5':
        Speed = 191;
        break;
      case '6':
        Speed = 204;
        break;
      case '7':
        Speed = 216;
        break;
      case '8':
        Speed = 229;
        break;
      case '9':
        Speed = 242;
        break;
      case 'q':
        Speed = 255;
        break;
      case 'W': // Turn on motor connected to pin 7.
        motor7On();
        break;
      case 'w': // Turn off motor connected to pin 7.
        motor7Off();
        break;
      case 'V': // Activate servo rotation.
        servoActive = true;
        break;
      case 'v': // Deactivate servo rotation.
        servoActive = false;
        myServo.write(90); // Set the servo to the neutral position.
        break;
    }
    Speedsec = Turnradius;
    if (brkonoff == 1) {
      brakeOn();
    } else {
      brakeOff();
    }
  }

  // If the servo is active, rotate it left and right.
  if (servoActive) {
    rotateServo();
  }
}

void forward() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speed);
}

void back() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speed);
}

void left() {
  analogWrite(in4, Speed);
  analogWrite(in1, Speed);
}

void right() {
  analogWrite(in3, Speed);
  analogWrite(in2, Speed);
}
void forwardleft() {
  analogWrite(in1, Speedsec);
  analogWrite(in3, Speed);
}
void forwardright() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speedsec);
}
void backright() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speedsec);
}
void backleft() {
  analogWrite(in2, Speedsec);
  analogWrite(in4, Speed);
}

void Stop() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void brakeOn() {
  //Here's the future use: an electronic braking system!
  // read the pushbutton input pin:
  buttonState = command;
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == 'S') {
      if (lastButtonState != buttonState) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, HIGH);
        delay(brakeTime);
        Stop();
      }
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState = buttonState;
  }

}
void brakeOff() {

}

void motor7On() {
  digitalWrite(motorPin7, HIGH); // Turn on the motor connected to pin 7.
}

void motor7Off() {
  digitalWrite(motorPin7, LOW); // Turn off the motor connected to pin 7.
}

void rotateServo() {
  // Non-blocking servo rotation.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    myServo.write(servoPos);
    servoPos += servoIncrement;

    // Reverse direction at 0 and 180 degrees.
    if (servoPos >= 180 || servoPos <= 0) {
      servoIncrement = -servoIncrement;
    }
  }
}
