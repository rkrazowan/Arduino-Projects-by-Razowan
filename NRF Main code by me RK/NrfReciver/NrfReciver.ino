#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define in1 3
#define in2 5  
#define in3 6
#define in4 9

RF24 radio(10, 9);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";//reciving and transmittion password

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
int motorSpeedA = 0;
int motorSpeedB = 0;

struct Data_Package {
  byte joy1_X;
  byte joy1_Y;
  byte j1Button;
  byte joy2_X;
  byte joy2_Y;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte pitch;
  byte roll;
};

Data_Package data; //Create a variable with the above structure

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
}
void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { 
    resetData(); 
  Serial.print("joy1_X: ");
  Serial.print(data.joy1_X);
  Serial.print("; joy1_Y: ");
  Serial.print(data.joy1_Y);
  Serial.print("; joy2_X: ");
  Serial.print(data.joy2_X); 
  Serial.print("; joy2_Y: ");
  Serial.print(data.joy2_Y);
  Serial.print("; j1Button: ");
  Serial.print(data.j1Button);
  Serial.print("; j2Button: ");
  Serial.print(data.j2Button);
  Serial.print("; tSwitch1: ");
  Serial.print(data.tSwitch1);
  Serial.print("; tSwitch2: ");
  Serial.print(data.tSwitch2);
  Serial.print("; pot1: ");
  Serial.print(data.pot1);
  Serial.print("; pot2: ");
  Serial.print(data.pot2);
  Serial.print("; Pitch: ");
  Serial.print(data.pitch);
  Serial.print("; Roll: ");
  Serial.println(data.roll);


  ///////////moving motor forward and backward
 if (data.joy1_Y < 120) {
 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
 
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
 
    motorSpeedA = map(data.joy1_Y, 120, 0, 0, 255);
    motorSpeedB = map(data.joy1_Y, 120, 0, 0, 255);
  }
  else if (data.joy1_Y > 135) {
 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
 
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    
    motorSpeedA = map(data.joy1_Y, 135, 1023, 0, 255);
    motorSpeedB = map(data.joy1_Y, 135, 1023, 0, 255);
  }
 
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
 
  if (data.joy2_X > 135) {
    
    int xMapped = map(data.joy2_X, 135, 0, 0, 255);
    
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (data.joy2_X < 120) {
    
    int xMapped = map(data.joy2_X, 120, 1023, 0, 255);
    
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
    
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
  
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B



}
















void resetData() {
  // Set initial default values
  data.joy1_X = 127;
  data.joy1_Y = 127;
  data.joy2_X = 127;
  data.joy2_Y = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.pitch = 0;
  data.roll = 0;
}
