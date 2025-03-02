#include <Keypad.h>

#define ROWS 4
#define COLS 4

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  // Row pin connections
byte colPins[COLS] = {5, 4, 3, 2};  // Column pin connections

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
    Serial.begin(9600);
    Serial.println("Keypad Test - Arduino Uno");
}

void loop() {
    char key = keypad.getKey();
    
    if (key) {
        Serial.print("Key Pressed: ");
        Serial.println(key);
    }
}
