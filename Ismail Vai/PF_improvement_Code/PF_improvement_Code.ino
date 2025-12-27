#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int xorPin = 8;       // XOR gate output connected to digital pin 8
const float period = 20000.0; // Time period for 50Hz signal (20 milliseconds)
unsigned long pulseWidth;
float phaseAngle;
float powerFactor;

int relaypin1 = 9;  // Relay for capacitor bank 1
int relaypin2 = 10; // Relay for capacitor bank 2
int relaypin3 = 11; // Relay for capacitor bank 3
int relaypin4 = 12; // Relay for capacitor bank 4

LiquidCrystal_I2C lcd(0x20, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(xorPin, INPUT);
  
  // Initialize relay pins as outputs
  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);
  pinMode(relaypin3, OUTPUT);
  pinMode(relaypin4, OUTPUT);
  
  // Initially turn off all relays (capacitor banks)
  digitalWrite(relaypin1, LOW);
  digitalWrite(relaypin2, LOW);
  digitalWrite(relaypin3, LOW);
  digitalWrite(relaypin4, LOW);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Power Factor Calc");
}

void loop() {
  float totalPhaseAngle = 0;
  int sampleCount = 10;

  // Measure phase angle multiple times and average it
  for (int i = 0; i < sampleCount; i++) {
    pulseWidth = pulseIn(xorPin, HIGH, 50000);  // Timeout added
    if (pulseWidth > 0) {
      totalPhaseAngle += (pulseWidth / period) * 360.0;
    }
    delay(100);  // Delay between samples
  }

  phaseAngle = totalPhaseAngle / sampleCount;
  powerFactor = cos(radians(phaseAngle));

  // Serial output for debugging
  Serial.print("Phase Angle (degrees): ");
  Serial.println(phaseAngle);
  Serial.print("Power Factor: ");
  Serial.println(powerFactor);

  // LCD output
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle: ");
  lcd.print(phaseAngle, 1);
  lcd.setCursor(0, 1);
  lcd.print("P.Factor: ");
  lcd.print(powerFactor, 2);

  // Relay control based on power factor
  if (powerFactor < 0.80) {
    // Turn on relays based on how low the power factor is
    if (powerFactor < 0.85 && powerFactor >= 0.75) {
      digitalWrite(relaypin1, HIGH);  // Turn on bank 1
      digitalWrite(relaypin2, LOW);   // Keep other banks off
      digitalWrite(relaypin3, LOW);
      digitalWrite(relaypin4, LOW);
    } 
    else if (powerFactor < 0.75 && powerFactor >= 0.70) {
      digitalWrite(relaypin1, HIGH);  // Turn on bank 1
      digitalWrite(relaypin2, HIGH);  // Turn on bank 2
      digitalWrite(relaypin3, LOW);   // Keep other banks off
      digitalWrite(relaypin4, LOW);
    } 
    else if (powerFactor < 0.70 && powerFactor >= 0.65) {
      digitalWrite(relaypin1, HIGH);  // Turn on bank 1
      digitalWrite(relaypin2, HIGH);  // Turn on bank 2
      digitalWrite(relaypin3, HIGH);  // Turn on bank 3
      digitalWrite(relaypin4, LOW);   // Keep bank 4 off
    } 
    else if (powerFactor < 0.65) {
      digitalWrite(relaypin1, HIGH);  // Turn on bank 1
      digitalWrite(relaypin2, HIGH);  // Turn on bank 2
      digitalWrite(relaypin3, HIGH);  // Turn on bank 3
      digitalWrite(relaypin4, HIGH);  // Turn on bank 4
    }
  } 
  else if (powerFactor >= 0.90) {
    // Turn off relays based on how high the power factor is
    if (powerFactor >= 0.90 && powerFactor < 0.95) {
      digitalWrite(relaypin1, LOW);  // Turn off bank 1
      digitalWrite(relaypin2, LOW);  // Turn off bank 2
      digitalWrite(relaypin3, LOW);  // Turn off bank 3
      digitalWrite(relaypin4, LOW);  // Turn off bank 4
    }
  }

  // Wait before next loop
  delay(1000);
}
