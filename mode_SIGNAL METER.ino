#define ledRed 4
#define ledYellow 2
#define ledGreen 13

int level = 0;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  Serial.begin(9600);
  Serial.println("Kirim angka 0-3");
}

void loop() {

  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c >= '0' && c <= '3') {
      level = c - '0';

      Serial.print("Level: ");
      Serial.println(level);
    }
  }

  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);

  if (level >= 1) digitalWrite(ledRed, HIGH);
  if (level >= 2) digitalWrite(ledYellow, HIGH);
  if (level >= 3) digitalWrite(ledGreen, HIGH);
}
