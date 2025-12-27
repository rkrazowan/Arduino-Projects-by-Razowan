#include <Adafruit_ADG72x.h>

Adafruit_ADG72x adg72x;

bool isADG728 = false; // which chip are we connected to?

void setup() {
  Serial.begin(115200);

  // Wait for serial port to open
  while (!Serial) {
    delay(1);
  }

  // Try with the ADG728 default address first...
  if (adg72x.begin(ADG728_DEFAULT_ADDR, &Wire)) {
    Serial.println("ADG728 found!");
    isADG728 = true;
  }
  // Maybe they have an ADG729?
  else if (adg72x.begin(ADG729_DEFAULT_ADDR, &Wire)) {
    Serial.println("ADG729 found!");
    isADG728 = false;
  }
  else {
    Serial.println("No ADG device found? Check wiring!");
    while (1); // Stop here if no device was found
  }
}

void loop() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("Selecting channel S");
    if (isADG728) {
      Serial.println(i+1);
    } else {
      Serial.print((i % 4) + 1);
      if (i < 4) Serial.println("A");
      else       Serial.println("B");
    }
    uint8_t bits = 1 << i; // Shift a '1' from LSB to MSB
    if (!adg72x.selectChannels(bits)) {
      Serial.println("Failed to set channels...");
    }
    delay(1000); // Delay 1 second between each shift
  }
}