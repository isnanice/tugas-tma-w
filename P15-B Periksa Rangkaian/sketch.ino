#include <WiFi.h>

const char NIM[] = "21120124120020";

// Konstanta pin (mengandung kesalahan sesuai skematik P15-B)
const int PIN_BTN = 12; // Kesalahan 1: strapping pin GPIO12 dengan pull-up luar
const int PIN_LED = 4;  // Kesalahan 2: LED tanpa resistor seri
const int PIN_POT = 26; // Kesalahan 3: ADC2 (GPIO26) dipakai bersamaan dengan Wi-Fi

const int AMBANG_ATAS  = 2400;
const int AMBANG_BAWAH = 2200;

bool modeAuto = false;
int outState = 0;
unsigned long lastStatusMs = 0;

void updateLogika() {
  int raw = analogRead(PIN_POT);
  if (raw < 0) raw = 0;
  int btn = (digitalRead(PIN_BTN) == LOW) ? 1 : 0;

  if (!modeAuto) {
    outState = btn;
  } else {
    if (raw >= AMBANG_ATAS) {
      outState = 1;
    } else if (raw <= AMBANG_BAWAH) {
      outState = 0;
    }
  }
  digitalWrite(PIN_LED, outState);
}

void cetakStatus() {
  updateLogika();
  int raw = analogRead(PIN_POT);
  if (raw < 0) raw = 0;
  int btn = (digitalRead(PIN_BTN) == LOW) ? 1 : 0;
  Serial.printf("MODE=%s RAW=%d BTN=%d OUT=%d\n",
                modeAuto ? "AUTO" : "MANUAL", raw, btn, outState);
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P15-B\n", NIM);

  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED, OUTPUT);
  analogReadResolution(12);

  // Mengaktifkan Wi-Fi (menyebabkan kanal ADC2 seperti GPIO26 gagal dibaca)
  WiFi.begin("Wokwi-GUEST", "");
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 5000) {
    delay(100);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.equalsIgnoreCase("MODE AUTO")) {
      modeAuto = true;
      Serial.println("OK");
    } else if (cmd.equalsIgnoreCase("MODE MANUAL")) {
      modeAuto = false;
      Serial.println("OK");
    } else if (cmd.equalsIgnoreCase("STATUS?")) {
      cetakStatus();
    } else if (cmd.length() > 0) {
      Serial.println("ERR");
    }
  }

  updateLogika();

  if (millis() - lastStatusMs >= 500) {
    lastStatusMs = millis();
    cetakStatus();
  }
}