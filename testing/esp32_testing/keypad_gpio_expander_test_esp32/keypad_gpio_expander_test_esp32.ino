#include <Wire.h>
#include <PCF8574.h>

#define I2C_ADDRESS 0x20  // Default PCF8574 I2C address

PCF8574 pcf(I2C_ADDRESS);

// Key mapping for 4x4 matrix keypad
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Row and Column mapping based on PCF8574 pins
uint8_t rowPins[4] = {4, 5, 6, 7};  // R1, R2, R3, R4 (PCF8574 GPIO numbers)
uint8_t colPins[4] = {0, 1, 2, 3};  // C1, C2, C3, C4

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // ESP32 default I2C pins (SDA: GPIO21, SCL: GPIO22)
  
  // Initialize PCF8574
  if (!pcf.begin()) {
    Serial.println("PCF8574 not found!");
    while (1);
  }

  // Set rows as outputs and columns as inputs
  for (int i = 0; i < 4; i++) {
    pcf.write(rowPins[i], HIGH);  // Set rows HIGH (inactive)
  }

  Serial.println("4x4 Keypad Test with PCF8574");
}

void loop() {
  for (int row = 0; row < 4; row++) {
    // Set the current row LOW
    for (int i = 0; i < 4; i++) {
      pcf.write(rowPins[i], (i == row) ? LOW : HIGH);
    }

    // Check columns
    for (int col = 0; col < 4; col++) {
      if (pcf.readButton(colPins[col]) == LOW) {  // Key pressed
        Serial.print("Key Pressed: ");
        Serial.println(keys[row][col]);

        // Debounce delay
        delay(200);

        // Wait for key release
        while (pcf.readButton(colPins[col]) == LOW);
      }
    }
  }
}
