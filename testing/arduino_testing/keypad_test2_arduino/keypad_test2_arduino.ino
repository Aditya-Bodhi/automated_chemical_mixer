#include <Keypad.h>

// Define the connections
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Connect to the row pins of the keypad
byte rowPins[ROWS] = {3, 2, 1, 0}; // R1-D3, R2-D2, R3-D1(tx), R4-D0(tx)
// Connect to the column pins of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; // C1-D7, C2-D6, C3-D5, C4-D4

// Define the keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Create the Keypad object
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }
}
