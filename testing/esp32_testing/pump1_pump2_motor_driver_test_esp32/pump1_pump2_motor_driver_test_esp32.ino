#define IN1  25  // Pump 1 ON
#define IN2  26  // Pump 1 OFF

#define IN3  27  // Pump 2 ON
#define IN4  14  // Pump 2 OFF

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  Serial.println("Pump 1 ON");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(5000);

  Serial.println("Pump 1 OFF");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(2000);

  Serial.println("Pump 2 ON");
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(5000);

  Serial.println("Pump 2 OFF");
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(2000);
}
