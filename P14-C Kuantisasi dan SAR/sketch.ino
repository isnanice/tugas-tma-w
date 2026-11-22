const char NIM[] = "21120124120020";

const int PIN_POT = 34;

void simulasiSAR(float vinMv, bool tampilLangkah) {
  if (vinMv < 0.0f) vinMv = 0.0f;
  if (vinMv > 3300.0f) vinMv = 3300.0f;

  float lsbMv = 3300.0f / 4096.0f;
  if (tampilLangkah) {
    Serial.printf("SAR untuk %.1f mV (1 LSB = %.3f mV)\n", vinMv, lsbMv);
  }

  uint16_t kode = 0;
  for (int bit = 11; bit >= 0; bit--) {
    uint16_t ujiKode = kode | (1U << bit);
    float ujiMv = ujiKode * lsbMv;
    bool simpan = (vinMv >= ujiMv);
    if (simpan) {
      kode = ujiKode;
    }
    if (tampilLangkah) {
      Serial.printf("  bit %2d: coba %4u (%7.1f mV) -> %s\n",
                    bit, ujiKode, ujiMv, simpan ? "simpan" : "buang");
    }
  }

  float hasilMv = kode * lsbMv;
  Serial.printf("Kode akhir %u, mewakili %.1f mV\n", kode, hasilMv);
}

void bacaPotensiometer() {
  long jumlah = 0;
  int nilaiMin = 4095;
  int nilaiMaks = 0;
  const int N = 16;

  for (int i = 0; i < N; i++) {
    int v = analogRead(PIN_POT);
    jumlah += v;
    if (v < nilaiMin) nilaiMin = v;
    if (v > nilaiMaks) nilaiMaks = v;
    delay(2);
  }

  int rata = jumlah / N;
  Serial.printf("Potensiometer: rata-rata %d, min %d, maks %d (selisih %d LSB)\n",
                rata, nilaiMin, nilaiMaks, nilaiMaks - nilaiMin);
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P14-C\n", NIM);
  analogReadResolution(12);
  Serial.println("Ketik tegangan mV (mis. 1650) atau P.");
}

void loop() {
  if (Serial.available() > 0) {
    String masukan = Serial.readStringUntil('\n');
    masukan.trim();
    if (masukan.length() == 0) return;

    if (masukan.equalsIgnoreCase("P")) {
      bacaPotensiometer();
    } else {
      float tegangan = masukan.toFloat();
      bool punyaDesimal = (masukan.indexOf('.') != -1);
      bool tampilDetail = (!punyaDesimal && abs(tegangan - 360.0f) < 0.01f);
      simulasiSAR(tegangan, tampilDetail);
    }
  }
}