#include <Wire.h>

const char NIM[] = "21120124120020";

const uint8_t ALAMAT_RTC = 0x69;

uint8_t bcdToDec(uint8_t val) {
  return (val / 16 * 10) + (val % 16);
}

void pindaiI2C() {
  Serial.println("Memindai bus I2C...");
  for (uint8_t addr = 1; addr <= 126; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  perangkat menjawab di 0x%02X\n", addr);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P13-D\n", NIM);

  Wire.begin(21, 22);
  Wire.setClock(100000);

  pindaiI2C();
}

void loop() {
  Wire.beginTransmission(ALAMAT_RTC);
  Wire.write(0x00); // pointer register detik DS1307
  if (Wire.endTransmission() != 0) {
    Serial.println("RTC tidak menjawab");
    delay(1000);
    return;
  }

  uint8_t n = Wire.requestFrom(ALAMAT_RTC, (uint8_t)3);
  if (n < 3) {
    Serial.println("RTC tidak menjawab");
  } else {
    uint8_t detik = bcdToDec(Wire.read() & 0x7F);
    uint8_t menit = bcdToDec(Wire.read());
    uint8_t jam   = bcdToDec(Wire.read() & 0x3F);
    Serial.printf("Jam RTC %02u:%02u:%02u\n", jam, menit, detik);
  }

  delay(1000);
}