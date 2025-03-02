#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the keymap (adjust if your keypad has a different layout)
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define the row and column pins
byte rowPins[ROWS] = {13, 12, 14, 27}; // Connect to keypad row pins
byte colPins[COLS] = {26, 25, 33, 32}; // Connect to keypad column pins

// Initialize keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200); // Start serial communication
  Serial.println("4x4 Keypad Test Initialized!");
}

void loop() {
  char key = keypad.getKey(); // Read key press

  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
  }
}
