#include <Wire.h>
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>

#define KEYPAD_I2C_ADDRESS 0x20  // PCF8574 I2C address
#define LCD_I2C_ADDRESS 0x27     // LCD I2C address

#define IN1  25  // Pump 1 ON
#define IN2  26  // Pump 1 OFF

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

int selectedIndex = 0;
const int volumes[] = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500};
const int pumpTimes[] = {2500, 5000, 6950, 8600, 10400, 12365, 14420, 16480, 18300, 20500};

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // ESP32 I2C pins

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  if (!pcf.begin()) {
    Serial.println("PCF8574 not found!");
    while (1);
  }

  lcd.init();
  lcd.backlight();

  for (int i = 0; i < 4; i++) {
    pcf.write(rowPins[i], HIGH);
  }

  showVolumeMenu();
}

void loop() {
  char key = getKey();
  if (key == 'A') {
    selectedIndex = (selectedIndex == 0) ? 9 : selectedIndex - 1;
    showVolumeMenu();
  } else if (key == 'B') {
    selectedIndex = (selectedIndex == 9) ? 0 : selectedIndex + 1;
    showVolumeMenu();
  } else if (key == '*') {
    dispenseLiquid(volumes[selectedIndex]);
  }
}

void showVolumeMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Volume:");
  lcd.setCursor(0, 1);
  lcd.print(volumes[selectedIndex]);
  lcd.print(" mL");
}

void dispenseLiquid(int volume) {
  int duration = 0;
  for (int i = 0; i < 10; i++) {
    if (volumes[i] == volume) {
      duration = pumpTimes[i];
      break;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing: ");
  lcd.print(volume);
  lcd.print(" mL");

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(duration);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done!");
  delay(2000);
  showVolumeMenu();
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
