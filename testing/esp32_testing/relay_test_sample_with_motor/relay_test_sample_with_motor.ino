#include <Wire.h>
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>

#define KEYPAD_I2C_ADDRESS 0x20  // PCF8574 address
#define LCD_I2C_ADDRESS    0x27  // LCD I2C address

// Pump control pins using L298N
#define IN1  25  // Pump 1 ON
#define IN2  26  // Pump 1 OFF
#define IN3  27  // Pump 2 ON
#define IN4  14  // Pump 2 OFF

#define MIXING_RELAY_PIN 32  // Active LOW relay to control motor

PCF8574 pcf(KEYPAD_I2C_ADDRESS);
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

uint8_t rowPins[4] = {4, 5, 6, 7};  // PCF8574 GPIOs for rows
uint8_t colPins[4] = {0, 1, 2, 3};  // PCF8574 GPIOs for columns

int containerA = 0, containerB = 0;
const int pumpTimes[] = {2500, 5000, 6950, 8600, 10400, 12365, 14420, 16480, 18300, 20500};
const int volumes[] =   {  50,  100,  150,  200,  250,  300,  350,  400,  450,  500};

const int mixingTime = 5000;  // in milliseconds

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL

  // Pump pins
  pinMode(IN1, OUTPUT); digitalWrite(IN1, LOW);
  pinMode(IN2, OUTPUT); digitalWrite(IN2, LOW);
  pinMode(IN3, OUTPUT); digitalWrite(IN3, LOW);
  pinMode(IN4, OUTPUT); digitalWrite(IN4, LOW);

  // Relay pin
  pinMode(MIXING_RELAY_PIN, OUTPUT);
  digitalWrite(MIXING_RELAY_PIN, HIGH); // Keep relay OFF initially (active LOW)

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
  while (true) {
    containerA = selectVolume("Select Volume A");
    containerB = selectVolume("Select Volume B");
    dispenseLiquids();
  }
}

void showWelcomeScreen() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME");
  lcd.setCursor(0, 1);
  lcd.print("Chemical Mixer");
  delay(2000);
}

int selectVolume(String message) {
  int index = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);

  while (true) {
    lcd.setCursor(0, 1);
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.print(volumes[index]);
    lcd.print(" mL ");

    char key = getKey();
    if (key == 'A') {
      index = (index > 0) ? index - 1 : 9;
    }
    else if (key == 'B') {
      index = (index < 9) ? index + 1 : 0;
    }
    else if (key == '*') {
      return volumes[index];
    }
  }
}

void dispenseLiquids() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing A...");
  pumpLiquid(containerA, IN1, IN2);
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing B...");
  pumpLiquid(containerB, IN3, IN4);
  delay(1000);

  // Start Mixing
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mixing...");
  digitalWrite(MIXING_RELAY_PIN, LOW);  // Relay ON (motor ON)
  delay(mixingTime);
  digitalWrite(MIXING_RELAY_PIN, HIGH); // Relay OFF (motor OFF)

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done!");
  delay(2000);
}

void pumpLiquid(int volume, int pumpOn, int pumpOff) {
  int duration = 0;
  for (int i = 0; i < 10; i++) {
    if (volumes[i] == volume) {
      duration = pumpTimes[i];
      break;
    }
  }

  if (duration > 0) {
    int stepSize = 5;
    int totalSteps = volume / stepSize;
    int stepTime = duration / totalSteps;

    digitalWrite(pumpOn, HIGH);
    digitalWrite(pumpOff, LOW);

    for (int dispensed = 5; dispensed <= volume; dispensed += stepSize) {
      lcd.setCursor(0, 1);
      lcd.print("Dispensed: ");
      lcd.print(dispensed);
      lcd.print(" mL ");
      delay(stepTime);
    }

    digitalWrite(pumpOn, LOW);
    digitalWrite(pumpOff, LOW);
  }
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
