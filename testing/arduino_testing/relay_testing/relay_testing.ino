#define RELAY_PIN 7  // Connect the relay IN pin to Arduino digital pin 7

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Start with relay OFF (active LOW)
}

void loop() {
  digitalWrite(RELAY_PIN, LOW);   // Turn relay ON
  delay(2000);                    // Wait 2 seconds
  digitalWrite(RELAY_PIN, HIGH);  // Turn relay OFF
  delay(2000);                    // Wait 2 seconds
}
