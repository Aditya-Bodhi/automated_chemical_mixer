#include <LiquidCrystal.h>

// Initialize the LCD with the pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Set up the LCD columns and rows
  lcd.begin(16, 2);
  
  // Display a message
  lcd.setCursor(0, 0);
  lcd.print("Hello, World!");
  
  lcd.setCursor(0, 1);
  lcd.print("Arduino Rocks!");
}

void loop() {
  // Scroll text for fun
  lcd.scrollDisplayLeft();
  delay(500);
}

