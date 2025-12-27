#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "Road to 214";
const char* password = "@@adhir767@@";

WebServer server(80);

// Motor control pins
const int motorForward = 14;
const int motorBackward = 27;
const int motorLeft = 26;
const int motorRight = 25;
const int motorSpeedA = 32;
const int motorSpeedB = 23; //enA = 23 because 32 not aviable in my esp32

// Servo control pin
const int servoPin = 13;
Servo myservo;

// Soil moisture sensor pin
const int soilMoisturePin = 34;

// Relay control pin for water pump
const int relayPin = 12;

// Variables to store the state
int speedValue = 0;
int servoValue = 90;
int soilMoistureValue = 0;

void setup() {
  Serial.begin(115200);

  // Initialize motor control pins
  pinMode(motorForward, OUTPUT);
  pinMode(motorBackward, OUTPUT);
  pinMode(motorLeft, OUTPUT);
  pinMode(motorRight, OUTPUT);
  pinMode(motorSpeedA, OUTPUT);
  pinMode(motorSpeedB, OUTPUT);

  // Initialize servo motor
  myservo.attach(servoPin);

  // Initialize relay pin
  pinMode(relayPin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());

  // Web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/forward", HTTP_GET, handleForward);
  server.on("/backward", HTTP_GET, handleBackward);
  server.on("/left", HTTP_GET, handleLeft);
  server.on("/right", HTTP_GET, handleRight);
  server.on("/stop", HTTP_GET, handleStop);
  server.on("/setSpeed", HTTP_GET, handleSetSpeed);
  server.on("/setServo", HTTP_GET, handleSetServo);
  server.on("/getSoilMoisture", HTTP_GET, handleGetSoilMoisture);
  server.on("/pumpOn", HTTP_GET, handlePumpOn);
  server.on("/pumpOff", HTTP_GET, handlePumpOff);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  soilMoistureValue = analogRead(soilMoisturePin);
  // Serial.print("Soil Moisture: ");
  // Serial.println(soilMoistureValue);
}

void handleRoot() {
  String htmlPage = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32 Car Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1>ESP32 Car Control</h1>
  <p>Speed: <input type="range" min="0" max="255" value="0" onchange="setSpeed(this.value)"></p>
  <p>Servo: <input type="range" min="0" max="180" value="90" onchange="setServo(this.value)"></p>
  <p><button onclick="sendCommand('forward')">Forward</button></p>
  <p><button onclick="sendCommand('backward')">Backward</button></p>
  <p><button onclick="sendCommand('left')">Left</button></p>
  <p><button onclick="sendCommand('right')">Right</button></p>
  <p><button onclick="sendCommand('stop')">Stop</button></p>
  <p>Soil Moisture: <span id="soilMoisture">0</span></p>
  <p><button onclick="sendCommand('pumpOn')">Pump ON</button></p>
  <p><button onclick="sendCommand('pumpOff')">Pump OFF</button></p>
  <script>
    function sendCommand(cmd) {
      fetch('/' + cmd);
    }
    function setSpeed(value) {
      fetch('/setSpeed?value=' + value);
    }
    function setServo(value) {
      fetch('/setServo?value=' + value);
    }
    setInterval(() => {
      fetch('/getSoilMoisture').then(response => response.text()).then(data => {
        document.getElementById('soilMoisture').innerText = data;
      });
    }, 1000);
  </script>
</body>
</html>)rawliteral";

  server.send(200, "text/html", htmlPage);
}

void handleForward() {
  digitalWrite(motorForward, HIGH);
  digitalWrite(motorBackward, LOW);
  digitalWrite(motorLeft, LOW);
  digitalWrite(motorRight, LOW);
  Serial.println("Moving Forward");
  server.send(200, "text/plain", "Moving Forward");
}

void handleBackward() {
  digitalWrite(motorForward, LOW);
  digitalWrite(motorBackward, HIGH);
  digitalWrite(motorLeft, LOW);
  digitalWrite(motorRight, LOW);
  Serial.println("Moving Backward");
  server.send(200, "text/plain", "Moving Backward");
}

void handleLeft() {
  digitalWrite(motorForward, LOW);
  digitalWrite(motorBackward, LOW);
  digitalWrite(motorLeft, HIGH);
  digitalWrite(motorRight, LOW);
  Serial.println("Turning Left");
  server.send(200, "text/plain", "Turning Left");
}

void handleRight() {
  digitalWrite(motorForward, LOW);
  digitalWrite(motorBackward, LOW);
  digitalWrite(motorLeft, LOW);
  digitalWrite(motorRight, HIGH);
  Serial.println("Turning Right");
  server.send(200, "text/plain", "Turning Right");
}

void handleStop() {
  digitalWrite(motorForward, LOW);
  digitalWrite(motorBackward, LOW);
  digitalWrite(motorLeft, LOW);
  digitalWrite(motorRight, LOW);
  Serial.println("Stopping");
  server.send(200, "text/plain", "Stopping");
}

void handleSetSpeed() {
  if (server.hasArg("value")) {
    speedValue = server.arg("value").toInt();
    analogWrite(motorSpeedA, speedValue);
    analogWrite(motorSpeedB, speedValue);
    Serial.print("Set Speed to: ");
    Serial.println(speedValue);
    server.send(200, "text/plain", "Speed set");
  }
}

void handleSetServo() {
  if (server.hasArg("value")) {
    servoValue = server.arg("value").toInt();
    myservo.write(servoValue);
    Serial.print("Set Servo to: ");
    Serial.println(servoValue);
    server.send(200, "text/plain", "Servo set");
  }
}

void handleGetSoilMoisture() {
  soilMoistureValue = analogRead(soilMoisturePin);
  server.send(200, "text/plain", String(soilMoistureValue));
}

void handlePumpOn() {
  digitalWrite(relayPin, HIGH);
  Serial.println("Water Pump ON");
  server.send(200, "text/plain", "Water Pump ON");
}

void handlePumpOff() {
  digitalWrite(relayPin, LOW);
  Serial.println("Water Pump OFF");
  server.send(200, "text/plain", "Water Pump OFF");
}
