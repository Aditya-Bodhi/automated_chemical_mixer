#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Initialize the LCD (I2C address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the keypad layout
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {13, 12, 14, 27}; // ESP32 GPIOs for rows
byte colPins[COLS] = {26, 25, 33, 32}; // ESP32 GPIOs for columns

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);  // Ensure correct I2C pins

    lcd.init();  // Correct function to initialize LCD
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Keypad Test");
    lcd.setCursor(0, 1);
    lcd.print("Press Key:");
}

void loop() {
    char key = keypad.getKey();
    if (key) {
        delay(200); // Debounce delay
        Serial.print("Key Pressed: ");
        Serial.println(key);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Pressed:");
        lcd.setCursor(0, 1);
        lcd.print(key);
    }
}
