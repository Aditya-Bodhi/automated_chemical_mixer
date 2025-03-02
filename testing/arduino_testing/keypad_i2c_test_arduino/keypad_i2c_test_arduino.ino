#include <Wire.h>
#include <PCF8574.h>

PCF8574 pcf8574(0x20);  // I2C address (adjust if necessary)

const char keys[4][4] = {
  {'D', '#', '0', '*'},
  {'C', '9', '8', '7'},
  {'B', '6', '5', '4'},
  {'A', '3', '2', '1'}
};


void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pcf8574.begin();  // Initialize the PCF8574

  // Set column pins as OUTPUTS
  for (int i = 4; i <= 7; i++) {
    pcf8574.write(i, HIGH);  // Set columns HIGH
  }

  // Set row pins as INPUTS
  for (int i = 0; i < 4; i++) {
    pcf8574.write(i, HIGH); // Enable pull-up resistors
  }
}

void loop() {
  for (int col = 0; col < 4; col++) {
    pcf8574.write(col + 4, LOW); // Set one column LOW

    for (int row = 0; row < 4; row++) {
      if (pcf8574.read(row) == LOW) { // Check if a key is pressed
        Serial.println(keys[row][col]);
        delay(300); // Debounce
      }
    }

    pcf8574.write(col + 4, HIGH); // Reset column to HIGH
  }
}
