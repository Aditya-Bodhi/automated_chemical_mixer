#define FLOW_SENSOR_PIN 13  // Connect to the yellow signal wire
volatile int pulseCount = 0;
float flowRate = 0.0;

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, RISING);
}

void loop() {
  pulseCount = 0;  // Reset count
  delay(1000);     // Measure pulses for 1 second
  
  flowRate = pulseCount * 2.25;  // YF-S401 gives ~2.25 pulses per mL (with water)
  
  Serial.print("Pulses: ");
  Serial.print(pulseCount);
  Serial.print(" | Estimated Flow (mL/min): ");
  Serial.println(flowRate);
}
