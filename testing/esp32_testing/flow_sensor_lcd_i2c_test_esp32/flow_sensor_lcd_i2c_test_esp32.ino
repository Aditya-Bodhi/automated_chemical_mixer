#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define FLOW_SENSOR_PIN 14  // Connect YF-S401 signal pin to GPIO 14
volatile int pulseCount = 0;
float flowRate;
unsigned long lastTime = 0;

// Initialize I2C LCD (Change 0x27 to 0x3F if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Interrupt service routine for counting pulses
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Flow Sensor Test");

  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
}

void loop() {
  if (millis() - lastTime > 1000) {  // Every 1 second
    detachInterrupt(FLOW_SENSOR_PIN); // Temporarily disable interrupt
    
    // Convert pulses to flow rate (Adjust the factor for your sensor)
    flowRate = (pulseCount / 7.5); // Assuming 7.5 pulses = 1 L/min

    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    // Display on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Flow Rate:");
    lcd.setCursor(0, 1);
    lcd.print(flowRate);
    lcd.print(" L/min");

    // Reset pulse counter
    pulseCount = 0;
    lastTime = millis();
    
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  }
}
