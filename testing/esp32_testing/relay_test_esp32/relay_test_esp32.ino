// Define relay pins
#define RELAY1  26  // Connect IN1 of the relay module to GPIO 26
#define RELAY2  27  // Connect IN2 of the relay module to GPIO 27

void setup() {
    // Set relay pins as output
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    
    // Ensure relays are OFF at startup
    digitalWrite(RELAY1, HIGH);  // Assuming LOW activates the relay
    digitalWrite(RELAY2, HIGH);
}

void loop() {
    // Turn Relay 1 ON
    digitalWrite(RELAY1, LOW);
    delay(1000); // 1 second
    
    // Turn Relay 1 OFF
    digitalWrite(RELAY1, HIGH);
    delay(1000); // 1 second
    
    // Turn Relay 2 ON
    digitalWrite(RELAY2, LOW);
    delay(1000); // 1 second
    
    // Turn Relay 2 OFF
    digitalWrite(RELAY2, HIGH);
    delay(1000); // 1 second
}
