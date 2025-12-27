//Mechano The Smart Floor Cleaning robot by Hobby Project

#include <Servo.h> //include servo.h library
Servo esc; 
const int trigPin1 = 2;
const int echoPin1 = 3;
const int trigPin2 = 4;
const int echoPin2 = 5;
const int trigPin3 = 8;
const int echoPin3 = 7;
int irpin =8;

long duration1;
long duration2;
long duration3;
int distanceleft;
int distancefront;
int distanceright; 

int a=0;
char cmnd;

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);// Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(irpin, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  esc.attach(13); 
  esc.writeMicroseconds(1000);
  delay(2000);
}
void loop() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distanceleft = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.println(distanceleft);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distancefront = duration2 * 0.034 / 2;
  Serial.print("Distance2: ");
  Serial.println(distancefront);
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
  val= analogRead(A0); 
  val= map(val, 0, 1023,1000,2000);
  esc.writeMicroseconds(val);
  

  if(s==HIGH)
  { 
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    delay(1000);
    a=1;
    }
  if ((a==0)&&(s==LOW)&&(distanceleft <= 15 && distancefront > 15 && distanceright <= 15) || (a==0)&&(s==LOW)&&(distanceleft > 15 && distancefront > 15 && distanceright > 15))
  {
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(12,LOW);
  }
  if ((a==1)&&(s==LOW)||(s==LOW)&&(distanceleft <= 15 && distancefront <= 15 && distanceright > 15)||(s==LOW)&&(distanceleft <= 15 && distancefront <= 15 && distanceright > 15)||(s==LOW)&& (distanceleft <= 15 && distancefront > 15 && distanceright > 15)||(distanceleft <= 15 && distancefront > 15 && distanceright > 15))
  {
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    delay(100);
    a=0;
  }
  if ((s==LOW)&&(distanceleft > 15 && distancefront <= 15 && distanceright <= 15) ||(s==LOW)&& (distanceleft > 15 && distancefront > 15 && distanceright <= 15) ||(s==LOW)&& (distanceleft > 15 && distancefront <= 15 && distanceright > 15) )
  {
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
  }
  
while(Serial.available() > 0)

 {
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  
  cmnd = Serial.read();
  Serial.println(cmnd);

  
if( cmnd == 'F') // Forward
    {
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);  
    }
  else if(cmnd == 'B') // Backward
    {
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH); 
    }
  
    else if(cmnd == 'R') //Right
    {
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    }
    else if(cmnd == 'L') //Left
    {
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH); 
    }
    else if(cmnd == 'I') //Forward Right
    {
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    }
  else if(cmnd == 'J') //Backward Right
    {
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    }
   else if(cmnd == 'G') //Forward Left
    {
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);  
    digitalWrite(12, LOW);
    }
  else if(cmnd == 'H') //Backward Left
    {
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH); 
    }}}
   
