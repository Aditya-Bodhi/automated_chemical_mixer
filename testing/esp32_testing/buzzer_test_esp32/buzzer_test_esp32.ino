// Define the GPIO pins for the buzzer
#define BUZZER_PIN_1 26
#define BUZZER_PIN_2 32

void setup() {
    // Set both GPIO pins as outputs
    pinMode(BUZZER_PIN_1, OUTPUT);
    pinMode(BUZZER_PIN_2, OUTPUT);
}

void loop() {
    // Activate the buzzer
    digitalWrite(BUZZER_PIN_1, HIGH);
    digitalWrite(BUZZER_PIN_2, LOW);
    delay(500); // Beep duration
    
    // Deactivate the buzzer
    digitalWrite(BUZZER_PIN_1, LOW);
    digitalWrite(BUZZER_PIN_2, LOW);
    delay(500); // Pause before next beep
}
