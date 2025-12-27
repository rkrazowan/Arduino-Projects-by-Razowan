// Mechano The Smart Floor Cleaning robot by Hobby Project

#include <Servo.h> // Include servo.h library
Servo esc; 

// Pin definitions
const int trigPin1 = 2;
const int echoPin1 = 3;
const int trigPin2 = 4;
const int echoPin2 = 5;
const int trigPin3 = 7;
const int echoPin3 = 8;
const int irpin = 12;

// Motor control pin variables
const int m1 = 9;
const int m2 = 10;
const int m3 = 11;
const int m4 = 6;

long duration1;
long duration2;
long duration3;
int distanceleft;
int distancefront;
int distanceright; 

int a = 0;
char cmnd;

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(irpin, INPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  esc.attach(13); 
  esc.writeMicroseconds(1000);
  delay(2000);
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
  
  int s = digitalRead(irpin);
  
  int val; 
  val = analogRead(A0); 
  val = map(val, 0, 1023, 1000, 2000);
  esc.writeMicroseconds(val);
  
  if (s == HIGH) { 
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    digitalWrite(m3, LOW);
    digitalWrite(m4, HIGH);
    delay(1000);
    a = 1;
  }

  if ((a == 0) && (s == LOW) && ((distanceleft <= 15 && distancefront > 15 && distanceright <= 15) || (distanceleft > 15 && distancefront > 15 && distanceright > 15))) {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
  }

  if ((a == 1 && s == LOW) || (s == LOW && distanceleft <= 15 && distancefront <= 15 && distanceright > 15) || (s == LOW && distanceleft <= 15 && distancefront <= 15 && distanceright > 15) || (s == LOW && distanceleft <= 15 && distancefront > 15 && distanceright > 15) || (distanceleft <= 15 && distancefront > 15 && distanceright > 15)) {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    digitalWrite(m3, LOW);
    digitalWrite(m4, HIGH);
    delay(100);
    a = 0;
  }

  if (s == LOW && ((distanceleft > 15 && distancefront <= 15 && distanceright <= 15) || (distanceleft > 15 && distancefront > 15 && distanceright <= 15) || (distanceleft > 15 && distancefront <= 15 && distanceright > 15))) {
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
  }

  while (Serial.available() > 0) {
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
    digitalWrite(m3, LOW);
    digitalWrite(m4, LOW);
  
    cmnd = Serial.read();
    Serial.println(cmnd);

    if (cmnd == 'F') { // Forward
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(m3, HIGH);
      digitalWrite(m4, LOW);  
    } else if (cmnd == 'B') { // Backward
      digitalWrite(m1, LOW);
      digitalWrite(m2, HIGH);
      digitalWrite(m3, LOW);
      digitalWrite(m4, HIGH); 
    } else if (cmnd == 'R') { // Right
      digitalWrite(m1, LOW);
      digitalWrite(m2, HIGH);
      digitalWrite(m3, HIGH);
      digitalWrite(m4, LOW);
    } else if (cmnd == 'L') { // Left
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(m3, LOW);
      digitalWrite(m4, HIGH); 
    } else if (cmnd == 'I') { // Forward Right
      digitalWrite(m1, HIGH);
      digitalWrite(m2, LOW);
      digitalWrite(m3, LOW);
      digitalWrite(m4, LOW);
    } else if (cmnd == 'J') { // Backward Right
      digitalWrite(m1, LOW);
      digitalWrite(m2, HIGH);
      digitalWrite(m3, LOW);
      digitalWrite(m4, LOW);
    } else if (cmnd == 'G') { // Forward Left
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(m3, HIGH);  
      digitalWrite(m4, LOW);
    } else if (cmnd == 'H') { // Backward Left
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      digitalWrite(m3, LOW);
      digitalWrite(m4, HIGH); 
    }
  }
}
