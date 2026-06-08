#define ledRed 4
#define ledYellow 2
#define ledGreen 13

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
}

void loop() {
  digitalWrite(ledRed, HIGH);
  delay(100);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, HIGH);
  delay(100);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, HIGH);
  delay(100);
  digitalWrite(ledGreen, LOW);
}