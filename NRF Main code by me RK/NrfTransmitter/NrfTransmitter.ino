
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> //https://github.com/tmrh20/RF24/
#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;

#define joybtn1 0  // Joystick button 1
#define joybtn2 1  // Joystick button 2
#define tgl1 7   // Toggle switch 1
#define tgl2 4   // Toggle switch 1
#define buzz 10  //Buzzer

RF24 radio(5, 6);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
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
  Serial.begin(9600);

  Serial.println("Initialize MPU6050");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  
  // radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  pinMode(joybtn1, INPUT_PULLUP);
  pinMode(joybtn2, INPUT_PULLUP);
  pinMode(tgl1, INPUT_PULLUP);
  pinMode(tgl2, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz,LOW);
  
  // initial default values
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


void loop() {
  
  // Read normalized values 
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch & Roll
  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  int roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
  
  // Read all analog inputs and map them to one Byte value
  data.joy1_X = map(analogRead(A1), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.joy1_Y = map(analogRead(A0), 0, 1023, 0, 255);
  data.joy2_X = map(analogRead(A2), 0, 1023, 0, 255);
  data.joy2_Y = map(analogRead(A3), 0, 1023, 0, 255);
  data.pot1 = map(analogRead(A7), 0, 1023, 0, 255);
  data.pot2 = map(analogRead(A6), 0, 1023, 0, 255);
  // Read all digital inputs
  data.j1Button = digitalRead(joybtn1);
  data.j2Button = digitalRead(joybtn2);
  data.tSwitch2 = digitalRead(tgl2);
  data.tSwitch1 = digitalRead(tgl1);
  data.pitch = (pitch);
  data.roll = (roll);
  
  radio.write(&data, sizeof(Data_Package)); // Send the whole data from the structure to the receiver
  Serial.println("&data");
  
}
