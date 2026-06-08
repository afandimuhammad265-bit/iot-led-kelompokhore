// C++ code
//

int led_merah = 2;
int led_kuning = 3;
int led_hijau = 5;

void setup()
{
  pinMode(led_merah, OUTPUT);
  pinMode(led_kuning, OUTPUT);
  pinMode(led_hijau, OUTPUT);
}

void loop()
{
  digitalWrite(led_merah, HIGH);
  
  digitalWrite(led_kuning, HIGH);
  
  digitalWrite(led_hijau, HIGH);
}