#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3); // RX, TX (to GPS TX and RX)

void setup() {
  Serial.begin(9600);        // Serial Monitor
  gpsSerial.begin(9600);     // GPS module
  Serial.println("GPS is initializing...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());

    if (gps.location.isUpdated()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
      Serial.println("-----------------------");
    }
  }
}
