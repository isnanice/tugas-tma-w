const char NIM[] = "21120124120020";

const uint32_t F_PWM       = 2000; // digit terakhir 0 -> 2000 Hz
const uint8_t  RES         = 15;   // RES tertinggi -> BERHASIL
const uint32_t DUTY_PERSEN = 20;   // dua digit terakhir NIM -> 20%

const int PIN_LED = 4;

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P11-D\n", NIM);

  bool ok = ledcAttach(PIN_LED, F_PWM, RES);
  if (!ok) {
    Serial.printf("ledcAttach(%lu Hz, %u bit): GAGAL\n", F_PWM, RES);
    Serial.println("Turunkan RES atau F_PWM, lalu jalankan ulang.");
  } else {
    uint32_t maksDuty = (1UL << RES) - 1;
    uint32_t duty = (maksDuty * DUTY_PERSEN) / 100UL;
    ledcWrite(PIN_LED, duty);
    Serial.printf("ledcAttach(%lu Hz, %u bit): BERHASIL\n", F_PWM, RES);
    Serial.printf("Rentang duty 0-%lu, ledcWrite(%lu) = %lu%%\n", maksDuty, duty, DUTY_PERSEN);
  }
}

void loop() {
  delay(1000);
}