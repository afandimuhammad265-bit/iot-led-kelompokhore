// C++ code
//

int led_merah = 4;
int led_kuning = 2;
int led_hijau = 13;

void setup()
{
  pinMode(led_merah, OUTPUT);
  pinMode(led_kuning, OUTPUT);
  pinMode(led_hijau, OUTPUT);
}

void loop()
{
  digitalWrite(led_merah, LOW);
  
  digitalWrite(led_kuning, LOW);
  
  digitalWrite(led_hijau, LOW);
}
