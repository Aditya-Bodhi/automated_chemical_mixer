#define IN3  27  // Pump 2 ON
#define IN4  14  // Pump 2 OFF

void setup() {
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  Serial.println("Pump 2 ON");
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(2500);

  Serial.println("Pump 2 OFF");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  while (true); // Stops further execution
}
