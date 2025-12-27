#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6

Servo topServo;
Servo bottomServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust 0x27 to your LCD address if necessary

int color = 0;
int lastColorPosition = 35;  // Store the last position of the bottom servo

// Color counters
int redCount = 0;
int greenCount = 0;
int yellowCount = 0;
int lightGreenCount = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  topServo.attach(10);
  bottomServo.attach(8);
  bottomServo.write(35);  // Initialize the bottom servo at the minimum angle of 35
  
  Serial.begin(9600);  // Initialize serial for monitoring

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Color  Sorting");
  lcd.setCursor(0, 1);
  lcd.print("     Project");

  delay(2000);  // Allow LCD to initialize
  lcd.clear();
}

void loop() {
  topServo.write(179);  // Initial position of top servo
  delay(500);

  // Move the top servo to the position of the object near the light sensor
  for (int i = 155; i > 97; i--) {
    topServo.write(i);
    delay(10);
  }
  delay(1000);

  color = readColor();  // Get the color from the sensor

  if (color == 5) {
    // Display "No Color Detected" if no color is found
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sorting  Project");
    lcd.setCursor(0, 1);
    lcd.print("No Color Detect");
  } else {
    updateColorCount(color);  // Update the color counts
    displayCounts();          // Show the counts of all colors on the LCD
  }

  Serial.print("Detected color: ");  // Output to serial monitor
  Serial.println(color);
  
  delay(10);

  // Ensure bottom servo starts at a minimum of 35 degrees and set positions based on color
  switch (color) {
    case 1:  // Red
      lastColorPosition = 35;
      bottomServo.write(35);
      redCount++;
      break;
    case 2:  // Green
      lastColorPosition = 75;
      bottomServo.write(75);
      greenCount++;
      break;
    case 3:  // Yellow
      lastColorPosition = 115;
      bottomServo.write(115);
      yellowCount++;
      break;
    case 4:  // Light Green
      lastColorPosition = 150;
      bottomServo.write(150);
      lightGreenCount++;
      break;
    case 5:  // No color detected
      bottomServo.write(lastColorPosition);  // Keep the bottom servo in the last position
      break;
  }
  
  delay(300);  // Uniform delay for servo movements

  // Return the top servo to its starting position
  for (int i = 105; i > 50; i--) {
    topServo.write(i);
    delay(2);
  }
  delay(200);

  for (int i = 50; i < 168; i++) {
    topServo.write(i);
    delay(5);
  }
  delay(100);

  color = 0;  // Reset color
}

// Custom Function - readColor()
int readColor() {
  int R = 0;
  int G = 0;
  int B = 0;
  
  // Read RED
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  R = pulseIn(sensorOut, LOW);

  // Read GREEN
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  G = pulseIn(sensorOut, LOW);

  // Read BLUE
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  B = pulseIn(sensorOut, LOW);

  int color = 0;
  int DifColor = abs(R - G);

  // Detect Red
  if (DifColor < 60 && DifColor > 30) {
    color = 1;  // Red
  }

  // Detect Green
  if (DifColor <= 2 && DifColor >= 0) {
    color = 2;  // Green
  }

  // Detect Yellow
  if (DifColor < 25 && DifColor >= 10) {
    color = 3;  // Yellow
  }

  // Detect Light Green
  if (DifColor < 10 && DifColor >= 3) {
    color = 4;  // Light Green
  }

  // No color detected
  if (R > 60) {
    color = 5;
  }

  return color;
  
}

// Function to update the color count based on the detected color
void updateColorCount(int color) {
  switch (color) {
    case 1:
      redCount++;
      break;
    case 2:
      greenCount++;
      break;
    case 3:
      yellowCount++;
      break;
    case 4:
      lightGreenCount++;
      break;
  }
}

// Function to display the counts of all colors on the LCD
void displayCounts() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("R:"); lcd.print(redCount);
  lcd.print(" G:"); lcd.print(greenCount);
  
  lcd.setCursor(0, 1);
  lcd.print("Y:"); lcd.print(yellowCount);
  lcd.print(" LG:"); lcd.print(lightGreenCount);
}