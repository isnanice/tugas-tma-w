const char NIM[] = "21120124120020";

const int F_PULSA     = 100; // 100 + 10 * 0 = 100 Hz
const int DUTY_PERSEN = 50;
const int SIBUK_MS    = 1;

const int PIN_PWM = 18;
const int PIN_IN  = 19;
const int PIN_LED = 4;

volatile uint32_t hitungISR = 0;
volatile uint32_t hasilISR = 0;
volatile bool gerbangSelesai = false;

hw_timer_t *timer = NULL;

void IRAM_ATTR onPulsa() {
  hitungISR++;
}

void IRAM_ATTR onTimer() {
  hasilISR = hitungISR;
  hitungISR = 0;
  gerbangSelesai = true;
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P09-E\n", NIM);

  pinMode(PIN_IN, INPUT);
  pinMode(PIN_LED, OUTPUT);

  ledcAttach(PIN_PWM, F_PULSA, 10);
  uint32_t dutyVal = (1023UL * DUTY_PERSEN) / 100UL;
  ledcWrite(PIN_PWM, dutyVal);

  uint32_t periodeUs = 1000000UL / F_PULSA;
  uint32_t lebarUs   = (periodeUs * DUTY_PERSEN) / 100UL;
  Serial.printf("Pulsa %d Hz, periode %lu us, lebar %lu us, jeda polling %d ms\n",
                F_PULSA, periodeUs, lebarUs, SIBUK_MS);

  attachInterrupt(digitalPinToInterrupt(PIN_IN), onPulsa, RISING);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0);
}

void loop() {
  static uint32_t hitungPolling = 0;
  static int statusSebelumnya = LOW;
  static uint32_t nomorGerbang = 0;
  static bool ledState = false;

  int statusSekarang = digitalRead(PIN_IN);
  if (statusSebelumnya == LOW && statusSekarang == HIGH) {
    hitungPolling++;
  }
  statusSebelumnya = statusSekarang;

  if (gerbangSelesai) {
    noInterrupts();
    uint32_t isrSnapshot = hasilISR;
    gerbangSelesai = false;
    interrupts();

    uint32_t pollSnapshot = hitungPolling;
    hitungPolling = 0;

    nomorGerbang++;
    ledState = !ledState;
    digitalWrite(PIN_LED, ledState);

    Serial.printf("Gerbang %lu: interupsi %lu pulsa | polling %lu pulsa\n",
                  nomorGerbang, isrSnapshot, pollSnapshot);
  }

  delay(SIBUK_MS);
}