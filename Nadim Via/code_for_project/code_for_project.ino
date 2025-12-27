#include <WiFi.h>
#include <HTTPClient.h>
#include <ZMPT101B.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi credentials
const char* ssid = "Infinix S5";
const char* password = "11112222";

// Google script ID
String GOOGLE_SCRIPT_ID = "AKfycbxhoJ24-nnzBLPm_TMYX9_pZPwFj4ELpQdNjlb203L7wbHlg6QFGNd0Incer9uBl6Ep";

// OLED Display SDA and SCL pins
#define OLED_SDA 4
#define OLED_SCL 5

// OLED Display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor and pin definitions
#define SAMPLES 300  // Number of samples for current sensor reading
#define ACS_Pin 35   // ACS712 current sensor pin for ESP32
#define SENSITIVITY 500.0f  // Sensitivity for voltage sensor
#define VREF 3.3
int vsensor = 34;  // Voltage sensor pin
ZMPT101B voltageSensor(vsensor, 50.0);

float correctionfactor = 0; // Set to 0 for initial testing
float Wh = 0;
unsigned long last_time = 0;
unsigned long current_time = 0;

float High_peak = 0;
float Low_peak = 4095;
float Amps_RMS;

#define LED_PIN 2  // Onboard LED pin

bool wifiConnected = false;  // Wi-Fi connection status

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ACS_Pin, INPUT);
  voltageSensor.setSensitivity(SENSITIVITY);

  // Initialize I2C and OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  // Attempt to connect to WiFi
  connectToWiFi();
}

void loop() {
  float ACcurrent = readACCurrentValue();  // Get AC current
  ACcurrent = ACcurrent-.1;
  float acvoltage = voltageSensor.getRmsVoltage();  // Get AC voltage
  acvoltage = acvoltage-1;
  float P = acvoltage * ACcurrent;  // Calculate power

  last_time = current_time;
  current_time = millis();
  Wh = Wh + P * ((current_time - last_time) / 3600000.0);  // Calculate energy in Wh

  // Display data on OLED
  displayData(acvoltage, ACcurrent, P, Wh);

  // If WiFi is connected, send data to Google Sheets
  if (wifiConnected) {
    sendDataToGoogleSheets(acvoltage, ACcurrent, P, Wh);
  }

  delay(1000);
}

void connectToWiFi() {
  // Attempt to connect to WiFi
  WiFi.begin(ssid, password);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi...");
  display.display();

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {  // 10-second timeout
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi Connected!");
    display.display();
    digitalWrite(LED_PIN, HIGH);  // Turn on LED to indicate connection
    delay(2000);  // Show the WiFi connected message for 2 seconds
  } else {
    wifiConnected = false;
    Serial.println("WiFi connection failed.");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi Not Connected");
    display.display();
    delay(2000);  // Show the WiFi not connected message for 2 seconds
  }
}

float readACCurrentValue() {
  read_Amps();  // Calculate the RMS current using ACS712 sensor
  return Amps_RMS;  // Return the RMS current
}

void read_Amps() {
  int cnt;
  High_peak = 0;
  Low_peak = 4095;

  for (cnt = 0; cnt < SAMPLES; cnt++) {
    float ACS_Value = analogRead(ACS_Pin);  // Read current sensor value
    if (ACS_Value > High_peak) {
      High_peak = ACS_Value;
    }
    if (ACS_Value < Low_peak) {
      Low_peak = ACS_Value;
    }
  }

  Amps_RMS = (High_peak - Low_peak) * 0.3536 * 0.06;  // Calculate RMS current
}

void displayData(float voltage, float current, float power, float energy) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("Voltage: ");
  display.print(voltage);
  display.println(" V");

  display.setCursor(0, 16);
  display.print("Current: ");
  display.print(current);
  display.println(" A");

  display.setCursor(0, 32);
  display.print("Power: ");
  display.print(power);
  display.println(" W");

  display.setCursor(0, 48);
  display.print("Energy: ");
  display.print(energy);
  display.println(" Wh");

  // Display WiFi connection status
  display.setCursor(0, 56);
  if (wifiConnected) {
    display.print("WiFi: Connected");
  } else {
    display.print("WiFi: Not Connected");
  }

  display.display();
  delay(1500);
}

void sendDataToGoogleSheets(float voltage, float current, float power, float energy) {
  String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?"
                    + "voltage=" + String(voltage) + "&current=" + String(current)
                    + "&power=" + String(power) + "&units=" + String(energy);

  HTTPClient http;
  http.begin(urlFinal.c_str());
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Payload: " + payload);
  }
  http.end();
}