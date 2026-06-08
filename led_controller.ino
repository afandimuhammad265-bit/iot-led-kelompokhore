/*
 * ============================================================
 *  LED CONTROLLER — 4 Mode
 *  Pin : ledRed = 4 | ledYellow = 2 | ledGreen = 13
 * ------------------------------------------------------------
 *  Kirim perintah via Serial Monitor (9600 baud):
 *    D  → Disco      (berkedip bergantian 100ms)
 *    N  → Nyala Semua
 *    M  → Mati Semua
 *    0  → Signal level 0 (semua mati)
 *    1  → Signal level 1 (merah)
 *    2  → Signal level 2 (merah + kuning)
 *    3  → Signal level 3 (semua nyala)
 *    T  → Traffic Light (merah 10s → kuning 2s → hijau 10s)
 * ============================================================
 */

#define ledRed    4
#define ledYellow 2
#define ledGreen  13

// ---- State ----
char  currentMode  = 'M';   // default: mati semua
int   signalLevel  = 0;
int   discoStep    = 0;
unsigned long prevMillis    = 0;
unsigned long trafficMillis = 0;
int   trafficPhase = 0;     // 0=merah, 1=kuning, 2=hijau

// Durasi traffic light (ms)
const unsigned long TRAFFIC_RED    = 10000;
const unsigned long TRAFFIC_YELLOW =  2000;
const unsigned long TRAFFIC_GREEN  = 10000;

// ---- Helper ----
void setLEDs(bool r, bool y, bool g) {
  digitalWrite(ledRed,    r ? HIGH : LOW);
  digitalWrite(ledYellow, y ? HIGH : LOW);
  digitalWrite(ledGreen,  g ? HIGH : LOW);
}

void allOff() { setLEDs(false, false, false); }
void allOn()  { setLEDs(true,  true,  true);  }

// ---- Setup ----
void setup() {
  pinMode(ledRed,    OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen,  OUTPUT);
  allOff();

  Serial.begin(9600);
  Serial.println("==============================");
  Serial.println("  LED Controller — 4 Mode");
  Serial.println("------------------------------");
  Serial.println("  D  = Disco");
  Serial.println("  N  = Nyala Semua");
  Serial.println("  M  = Mati Semua");
  Serial.println("  0-3= Signal Meter");
  Serial.println("  T  = Traffic Light");
  Serial.println("==============================");
}

// ---- Loop ----
void loop() {

  // Baca perintah dari Serial
  if (Serial.available() > 0) {
    char c = (char)Serial.read();

    if (c == 'D' || c == 'd') {
      currentMode = 'D';
      discoStep   = 0;
      prevMillis  = millis();
      Serial.println("[MODE] Disco");

    } else if (c == 'N' || c == 'n') {
      currentMode = 'N';
      allOn();
      Serial.println("[MODE] Nyala Semua");

    } else if (c == 'M' || c == 'm') {
      currentMode = 'M';
      allOff();
      Serial.println("[MODE] Mati Semua");

    } else if (c >= '0' && c <= '3') {
      currentMode  = 'S';
      signalLevel  = c - '0';
      Serial.print("[MODE] Signal Meter — Level: ");
      Serial.println(signalLevel);

    } else if (c == 'T' || c == 't') {
      currentMode  = 'T';
      trafficPhase = 0;
      trafficMillis = millis();
      setLEDs(true, false, false);   // mulai: merah
      Serial.println("[MODE] Traffic Light");
      Serial.println("  >> Merah 10s");
    }
  }

  // ---- Jalankan mode aktif ----

  // --- DISCO ---
  if (currentMode == 'D') {
    unsigned long now = millis();
    if (now - prevMillis >= 100) {
      prevMillis = now;
      discoStep  = (discoStep + 1) % 3;
      setLEDs(discoStep == 0, discoStep == 1, discoStep == 2);
    }
  }

  // --- SIGNAL METER ---
  if (currentMode == 'S') {
    setLEDs(signalLevel >= 1, signalLevel >= 2, signalLevel >= 3);
  }

  // --- TRAFFIC LIGHT (non-blocking) ---
  if (currentMode == 'T') {
    unsigned long now = millis();
    unsigned long dur = (trafficPhase == 0) ? TRAFFIC_RED
                      : (trafficPhase == 1) ? TRAFFIC_YELLOW
                      :                       TRAFFIC_GREEN;

    if (now - trafficMillis >= dur) {
      trafficMillis = now;
      trafficPhase  = (trafficPhase + 1) % 3;

      if (trafficPhase == 0) {
        setLEDs(true, false, false);
        Serial.println("  >> Merah 10s");
      } else if (trafficPhase == 1) {
        setLEDs(false, true, false);
        Serial.println("  >> Kuning 2s");
      } else {
        setLEDs(false, false, true);
        Serial.println("  >> Hijau 10s");
      }
    }
  }
}
