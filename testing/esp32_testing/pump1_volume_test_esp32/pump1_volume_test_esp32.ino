

#define IN1  25  // Pump 1 ON
#define IN2  26  // Pump 1 OFF

#define IN3  27  // Pump 2 ON
#define IN4  14  // Pump 2 OFF

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ensure both pumps are OFF at startup
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  delay(500);  // Allow time to stabilize
}

void loop() {
  Serial.println("Pump 1 ON");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(5000);  // Run Pump 1 for 5 seconds

  Serial.println("Pump 1 OFF");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  while (1);  // Stop further execution
}
