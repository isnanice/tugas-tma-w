const char NIM[] = "21120124120020";

const long BAUD_KIRIM  = 1200; // digit terakhir 0 -> 1200 bps
const long BAUD_TERIMA = 600;  // Tahap 3: 0.5 x BAUD_KIRIM
const char* PESAN = "NIM0020\n";

const int PIN_TX2 = 17;
const int PIN_RX1 = 26;

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P12-D\n", NIM);

  Serial1.begin(BAUD_TERIMA, SERIAL_8N1, PIN_RX1, -1);
  Serial2.begin(BAUD_KIRIM, SERIAL_8N1, -1, PIN_TX2);
  delay(200);
}

void loop() {
  while (Serial1.available()) Serial1.read();

  int panjang = strlen(PESAN);
  unsigned long hitunganUs = (unsigned long)panjang * 10UL * 1000000UL / BAUD_KIRIM;

  unsigned long t0 = micros();
  Serial2.print(PESAN);
  Serial2.flush();
  unsigned long lamaUs = micros() - t0;

  delay(50);

  String diterima = "";
  while (Serial1.available()) {
    uint8_t b = (uint8_t)Serial1.read();
    if (b >= 32 && b <= 126) {
      diterima += (char)b;
    } else {
      char buf[8];
      snprintf(buf, sizeof(buf), "[%02X]", b);
      diterima += buf;
    }
  }

  Serial.printf("Kirim %d byte pada %ld bps: %lu us (hitungan 8N1: %lu us)\n",
                panjang, BAUD_KIRIM, lamaUs, hitunganUs);
  Serial.printf("Diterima UART1: %s\n", diterima.c_str());
  delay(2000);
}