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
int speed = 0;

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
  Serial.print(data.joy1_Y);/////its
  Serial.print("; joy2_X: ");
  Serial.print(data.joy2_X); /////its
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
 if (data.joy1_Y >135) {
  
  int speedY = map(data.joy1_Y, 135, 0, 0, 255);

    digitalWrite(in1, speedY);      //L_F   (Left Forward)
    digitalWrite(in2, LOW);         
    digitalWrite(in3, speedY);       //R_F   (Right Forward)
    digitalWrite(in4, LOW);
 
    
  }
  else if (data.joy1_Y <120) {    ///Backward
    
    int speedY = map(data.joy1_Y, 120, 255, 0, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, speedY);    //L_B   (Left Backward)
    digitalWrite(in3, LOW);
    digitalWrite(in4, speedY);    //R_B   (Right Backward)
    
  }
 
 
   ///left side movement
  if (data.joy2_X < 120) { 
    
    int speedX = map(data.joy2_X, 120, 255, 0, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, speedX);
    digitalWrite(in3, speedX);
    digitalWrite(in4, LOW);
  }

  ///Right side movement
 else if (data.joy2_X > 135 ) {
    
    int speedX = map(data.joy2_X, 135, 255, 0, 255);
    digitalWrite(in1, speedX);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, speedX);
 }


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

















