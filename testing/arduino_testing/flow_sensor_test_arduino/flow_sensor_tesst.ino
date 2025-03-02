volatile int pulseCount = 0;  // Stores the number of pulses from the sensor

// Interrupt function to count pulses
void countPulses() {
    pulseCount++;
}

void setup() {
    Serial.begin(9600);   // Start serial communication
    pinMode(2, INPUT_PULLUP);  // Set pin 2 as input with internal pull-up resistor
    attachInterrupt(digitalPinToInterrupt(2), countPulses, RISING);  // Attach interrupt on pin 2
}

void loop() {
    Serial.print("Pulse Count: ");
    Serial.println(pulseCount);  // Print pulse count every second
    delay(1000);
}

