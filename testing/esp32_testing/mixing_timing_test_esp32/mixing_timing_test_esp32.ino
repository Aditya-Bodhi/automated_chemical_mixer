#include <Wire.h>
#include <PCF8574.h>
#include <LiquidCrystal_I2C.h>

// I2C Addresses
#define KEYPAD_I2C_ADDRESS 0x20
#define LCD_I2C_ADDRESS 0x27

// Pump control pins (L298N)
#define IN1  25
#define IN2  26
#define IN3  27
#define IN4  14

// Relay control pin (Active LOW)
#define RELAY_PIN 32

PCF8574 pcf(KEYPAD_I2C_ADDRESS);
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

uint8_t rowPins[4] = {4, 5, 6, 7};  // PCF8574 row pins
uint8_t colPins[4] = {0, 1, 2, 3};  // PCF8574 column pins

int containerA = 0, containerB = 0;
int mixMinutes = 0, mixSeconds = 0;

const int pumpTimes[] = {2500, 5000, 6950, 8600, 10400, 12365, 14420, 16480, 18300, 20500};
const int volumes[] = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500};

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(RELAY_PIN, HIGH);  // Relay off (active LOW)

  if (!pcf.begin()) {
    Serial.println("PCF8574 not found!");
    while (1);
  }

  lcd.init();
  lcd.backlight();

  for (int i = 0; i < 4; i++) pcf.write(rowPins[i], HIGH);

  showWelcomeScreen();
}

void loop() {
  while (true) {
    containerA = selectVolume("Select Volume A");
    containerB = selectVolume("Select Volume B");
    dispenseLiquids();
    getMixingTime();
    startMixingCountdown();
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
    lcd.print("         ");
    lcd.setCursor(0, 1);
    lcd.print(volumes[index]);
    lcd.print(" mL");

    char key = getKey();
    if (key == 'A') index = (index > 0) ? index - 1 : 9;
    else if (key == 'B') index = (index < 9) ? index + 1 : 0;
    else if (key == '*') return volumes[index];
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
}

void pumpLiquid(int volume, int pin1, int pin2) {
  int duration = 0;
  for (int i = 0; i < 10; i++) {
    if (volumes[i] == volume) {
      duration = pumpTimes[i];
      break;
    }
  }

  if (duration > 0) {
    int step = 5;
    int steps = volume / step;
    int stepTime = duration / steps;

    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);

    for (int disp = 5; disp <= volume; disp += step) {
      lcd.setCursor(0, 1);
      lcd.print("Dispensed: ");
      lcd.print(disp);
      lcd.print(" mL ");
      delay(stepTime);
    }

    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
}

void getMixingTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter time (m):");
  mixMinutes = getNumericInput();

  if (mixMinutes > 30) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Limit is 30m");
    delay(2000);
    getMixingTime();
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter time (s):");
  mixSeconds = getNumericInput();

  int totalSec = mixMinutes * 60 + mixSeconds;
  if (totalSec > 1800) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Limit is 30");
    delay(2000);
    getMixingTime();
    return;
  }

  mixMinutes = totalSec / 60;
  mixSeconds = totalSec % 60;
}

void startMixingCountdown() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mixing");

  digitalWrite(RELAY_PIN, LOW); // Motor ON

  for (int t = mixMinutes * 60 + mixSeconds; t >= 0; t--) {
    int min = t / 60;
    int sec = t % 60;
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(min);
    lcd.print("m ");
    lcd.print(sec);
    lcd.print("s   ");
    delay(1000);
  }

  digitalWrite(RELAY_PIN, HIGH); // Motor OFF

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mixing Done");
  delay(3000);
}

int getNumericInput() {
  String input = "";
  char key;
  lcd.setCursor(0, 1);
  lcd.print("> ");

  while (true) {
    key = getKey();
    if (key >= '0' && key <= '9') {
      input += key;
      lcd.print(key);
    } else if (key == '*') {
      if (input.length() == 0) continue;
      return input.toInt();
    }
  }
}

char getKey() {
  for (int row = 0; row < 4; row++) {
    for (int i = 0; i < 4; i++) pcf.write(rowPins[i], (i == row) ? LOW : HIGH);

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
