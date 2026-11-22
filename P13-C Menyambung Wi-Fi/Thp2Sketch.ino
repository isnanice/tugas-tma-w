#include <WiFi.h>
#include <time.h>

const char NIM[] = "21120124120020";

const char* SSID = "Wokwi-020";
const char* PASS = "";

bool tersambung = false;

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P13-C\n", NIM);

  Serial.printf("Menyambung ke %s ...\n", SSID);
  unsigned long tMulai = millis();
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED && (millis() - tMulai) < 10000) {
    delay(100);
  }

  unsigned long lamaMs = millis() - tMulai;
  if (WiFi.status() == WL_CONNECTED) {
    tersambung = true;
    Serial.printf("Tersambung dalam %lu ms | IP %s | RSSI %d dBm\n",
                  lamaMs, WiFi.localIP().toString().c_str(), WiFi.RSSI());
    Serial.println("Menunggu jawaban server waktu ...");
    configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  } else {
    Serial.printf("Gagal setelah %lu ms. Periksa nama jaringan.\n", lamaMs);
  }
}

void loop() {
  if (!tersambung) {
    delay(1000);
    return;
  }

  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 1000)) {
    Serial.printf("Jam jaringan (WIB): %02d:%02d:%02d\n",
                  timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
  delay(2000);
}