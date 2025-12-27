#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

// Define sensor pins
#define LDR_PIN 34
#define IR_PIN 35
#define DHT_PIN 16
#define SDA_PIN 4
#define SCL_PIN 5

// Initialize DHT sensor
#define DHTTYPE DHT11  // or DHT22 depending on your sensor type
DHT dht(DHT_PIN, DHTTYPE);

// Initialize LCD with I2C address 0x27 (change if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize DHT sensor
  dht.begin();

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();

  // Print initial message to LCD
  lcd.setCursor(0, 0);
  lcd.print("Sensor Values:");
}

void loop() {
  // Read sensor values
  int ldrValue = analogRead(LDR_PIN);
  int irValue = digitalRead(IR_PIN);
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Display values on Serial Monitor
  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | IR: ");
  Serial.print(irValue);
  Serial.print(" | Temp: ");
  Serial.print(temp);
  Serial.print("C | Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Display values on LCD
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(ldrValue);
  lcd.print(" IR:");
  lcd.print(irValue);
  lcd.print(" T:");
  lcd.print(temp, 1);
  lcd.print("C");

  delay(2000);  // Update every 2 seconds
}
