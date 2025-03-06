#include <Wire.h> //test 1
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>

#define KEYPAD_I2C_ADDRESS 0x20  // PCF8574 I2C address
#define LCD_I2C_ADDRESS 0x27     // LCD I2C address (change if needed)

PCF8574 pcf(KEYPAD_I2C_ADDRESS);
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

char keys[4][4] = {  
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

uint8_t rowPins[4] = {4, 5, 6, 7};  // R1, R2, R3, R4 (PCF8574 GPIO numbers)
uint8_t colPins[4] = {0, 1, 2, 3};  // C1, C2, C3, C4

int containerA = 0, containerB = 0, mixTime = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // ESP32 default I2C pins

  if (!pcf.begin()) {
    Serial.println("PCF8574 not found!");
    while (1);
  }

  lcd.init();
  lcd.backlight();
  
  for (int i = 0; i < 4; i++) {
    pcf.write(rowPins[i], HIGH);
  }

  showWelcomeScreen();
}

void loop() {
  getUserInput();
  startMixing();
}

void showWelcomeScreen() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME");
  lcd.setCursor(0, 1);
  lcd.print("Chemical Mixer");
  delay(2000);
}

void getUserInput() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter A (ml):");
  containerA = getNumberInput();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter B (ml):");
  containerB = getNumberInput();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Mix Time(s):");
  mixTime = getNumberInput();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press any key");
  lcd.setCursor(0, 1);
  lcd.print("to start mixing");
  while (!getKey());
}

void startMixing() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mixing...");
  
  for (int i = mixTime; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Time left: ");
    lcd.print(i);
    lcd.print("s ");
    delay(1000);
  }

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Mixing Done");
  delay(2000);
}

char getKey() {
  for (int row = 0; row < 4; row++) {
    for (int i = 0; i < 4; i++) {
      pcf.write(rowPins[i], (i == row) ? LOW : HIGH);
    }
    for (int col = 0; col < 4; col++) {
      if (pcf.readButton(colPins[col]) == LOW) {
        delay(200);
        while (pcf.readButton(colPins[col]) == LOW);
        return keys[row][col];
      }
    }
  }
  return 0;
}

int getNumberInput() {
  int num = 0;
  char key;
  while (true) {
    key = getKey();
    if (key) {
      if (key >= '0' && key <= '9') {
        num = num * 10 + (key - '0');
        lcd.setCursor(0, 1);
        lcd.print(num);
      } else if (key == '#') {
        return num;
      }
    }
  }
}
