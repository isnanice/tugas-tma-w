const char NIM[] = "21120124120020";

const int PIN_BTN = 27;
const int PIN_LED = 4;
const int PIN_POT = 34;

const int AMBANG_ATAS  = 2400;
const int AMBANG_BAWAH = 2200;

bool modeAuto = false;
int outState = 0;

unsigned long lastStatusMs  = 0;
unsigned long lastKinerjaMs = 0;
uint32_t loopCounter = 0;

void updateLogika() {
  int raw = analogRead(PIN_POT);
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
  int btn = (digitalRead(PIN_BTN) == LOW) ? 1 : 0;
  Serial.printf("MODE=%s RAW=%d BTN=%d OUT=%d\n",
                modeAuto ? "AUTO" : "MANUAL", raw, btn, outState);
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P15-A\n", NIM);

  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  analogReadResolution(12);
}

void loop() {
  loopCounter++;

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

  unsigned long now = millis();
  if (now - lastStatusMs >= 500) {
    lastStatusMs = now;
    cetakStatus();
  }

  if (now - lastKinerjaMs >= 5000) {
    lastKinerjaMs = now;
    Serial.printf("# kinerja: %lu iterasi/5s\n", loopCounter);
    loopCounter = 0;
  }
}