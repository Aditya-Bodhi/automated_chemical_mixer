#define FLOW_SENSOR_PIN 5  // Change this if using a different pin

volatile int pulseCount = 0;  
unsigned long oldTime = 0;

// Interrupt function to count pulses
void IRAM_ATTR pulseCounter() {
    pulseCount++;
}

void setup() {
    Serial.begin(115200);
    
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);

    Serial.println("YF-S401 Airflow Sensor Test");
}

void loop() {
    if (millis() - oldTime > 1000) {  // Update every 1 second
        detachInterrupt(FLOW_SENSOR_PIN);
        
        Serial.print("Pulses per second: ");
        Serial.println(pulseCount);  // Show raw pulse count
        
        Serial.println("----------------------");
        
        pulseCount = 0;  
        oldTime = millis();  

        attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
    }
}
