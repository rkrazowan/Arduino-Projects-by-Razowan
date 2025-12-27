#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// Pin Definitions
const int voltagePin = 3;  // PT signal (Zero crossing detection)
const int currentPin = 6;  // CT signal (Zero crossing detection)
const int relayPins[] = {10, 11, 12, 13};  // Relays controlling capacitor banks

// Variables to store time values of zero-crossing
volatile unsigned long voltageZeroTime = 0;
volatile unsigned long currentZeroTime = 0;

// Variables for phase difference and power factor
volatile unsigned long phaseDifference = 0;
float powerFactor = 0.0;

// Moving average variables
float avgPowerFactor = 0;
const int numSamples = 10;
float powerFactorSamples[numSamples];
int sampleIndex = 0;

// Constants
const float frequency = 50.0;  // Frequency of AC mains (50Hz)
const unsigned long timePeriod = 1000000 / frequency; // Time period in microseconds

// Debounce time for zero-crossing detection
volatile unsigned long lastVoltageZeroTime = 0;
volatile unsigned long lastCurrentZeroTime = 0;
const unsigned long debounceTime = 500;  // Minimum time between zero crossings (in microseconds)

// Initialize the LCD with I2C address 0x27 (might vary, check with I2C scanner)
LiquidCrystal_I2C lcd(0x20, 16, 2);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the LCD and turn on the backlight
  lcd.init();
  lcd.backlight();

  // Set relay pins as OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW);  // Initially turn off all relays
  }

  // Attach interrupts for zero-crossing detection
  attachInterrupt(digitalPinToInterrupt(voltagePin), voltageZeroCross, RISING);
  attachInterrupt(digitalPinToInterrupt(currentPin), currentZeroCross, RISING);

  // Initialize power factor sample array
  for (int i = 0; i < numSamples; i++) {
    powerFactorSamples[i] = 1.0;  // Start with PF = 1 (ideal case)
  }
}

void loop() {
  // Calculate power factor based on phase difference
  noInterrupts();  // Disable interrupts while accessing shared variables
  unsigned long phase = phaseDifference;
  interrupts();  // Enable interrupts after access

  // Convert phase difference (in microseconds) to phase angle
  float phaseAngle = (phase * 360.0) / timePeriod;

  // Calculate power factor (cosine of phase angle)
  powerFactor = cos(radians(phaseAngle));

  // Store the current power factor in the sample array
  powerFactorSamples[sampleIndex] = powerFactor;
  sampleIndex = (sampleIndex + 1) % numSamples;

  // Calculate the average power factor
  avgPowerFactor = 0;
  for (int i = 0; i < numSamples; i++) {
    avgPowerFactor += powerFactorSamples[i];
  }
  avgPowerFactor /= numSamples;

  // Display power factor on the LCD
  lcd.setCursor(0, 0);
  lcd.print("PF: ");
  lcd.print(avgPowerFactor, 2);  // Display average power factor with 2 decimals

  lcd.setCursor(0, 1);
  lcd.print("R: ");  // Display relay statuses
  
  // Control the capacitor banks based on power factor and update relay status
  bool relayStatus[4];
  
  // Control relays based on power factor thresholds
  relayStatus[0] = avgPowerFactor < 0.9;
  relayStatus[1] = avgPowerFactor < 0.8;
  relayStatus[2] = avgPowerFactor < 0.7;
  relayStatus[3] = avgPowerFactor < 0.6;

  // Write relay control logic
  for (int i = 0; i < 4; i++) {
    digitalWrite(relayPins[i], relayStatus[i] ? HIGH : LOW);
  }

  // Display relay statuses on the LCD
  for (int i = 0; i < 4; i++) {
    lcd.print(relayStatus[i] ? "1" : "0");
  }

  // Serial debug output
  Serial.print("Phase Difference (us): ");
  Serial.println(phaseDifference);

  Serial.print("Phase Angle (degrees): ");
  Serial.println(phaseAngle);

  Serial.print("Power Factor: ");
  Serial.println(avgPowerFactor);

  // Delay for LCD update
  delay(1000);
}


// ISR for voltage zero-crossing detection
void voltageZeroCross() {
  unsigned long now = micros();
  if (now - lastVoltageZeroTime > debounceTime) {
    voltageZeroTime = now;
    lastVoltageZeroTime = now;
    updatePhaseDifference();
  }
}

// ISR for current zero-crossing detection
void currentZeroCross() {
  unsigned long now = micros();
  if (now - lastCurrentZeroTime > debounceTime) {
    currentZeroTime = now;
    lastCurrentZeroTime = now;
    updatePhaseDifference();
  }
}

// Function to calculate phase difference
void updatePhaseDifference() {
  // Calculate the phase difference between voltage and current zero-crossings
  phaseDifference = abs(voltageZeroTime - currentZeroTime);
}
