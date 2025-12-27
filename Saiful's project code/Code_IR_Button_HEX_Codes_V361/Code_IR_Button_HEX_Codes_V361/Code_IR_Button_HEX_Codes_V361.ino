#include <IRremote.h>

int IR_RECV_PIN = 34;  // D35 Update the pin as per circuit

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("IR Receiver is ready");
}

void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.print("Received value: 0x");
    Serial.println(results.value, HEX); // Print the HEX code
    irrecv.resume(); // Receive the next value
    delay(100); // Short delay to stabilize readings
  }
}
