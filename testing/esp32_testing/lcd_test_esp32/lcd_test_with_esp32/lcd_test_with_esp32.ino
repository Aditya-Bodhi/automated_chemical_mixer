#include <LiquidCrystal.h>

// Define LCD pin connections
const int rs = 21, en = 22, d4 = 18, d5 = 19, d6 = 23, d7 = 5;

// Initialize LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2); // Initialize 16x2 LCD
  lcd.print("Hello, ESP32!");  // Print on first row
  lcd.setCursor(0, 1);         // Move cursor to second row
  lcd.print("LCD Test Done!");
}

void loop() {
  // Nothing to do here
}
