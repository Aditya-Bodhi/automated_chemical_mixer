#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address (default is 0x27, try 0x3F if it doesn’t work)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Wire.begin(21, 22); // SDA = GPIO 21, SCL = GPIO 22 (ESP32 default I2C pins)
    lcd.init();         // Initialize the LCD
    lcd.backlight();    // Turn on the backlight

    // Display test message
    lcd.setCursor(0, 0);
    lcd.print("ESP32 with I2C");
    lcd.setCursor(0, 1);
    lcd.print("LCD Test OK!");
}

void loop() {
    // Nothing here, the message stays on the screen
}
