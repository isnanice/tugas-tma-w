const char NIM[] = "21120124120020";

const int AMBANG_BAWAH = 2000; // 2000 + 20 * 0 = 2000
const int AMBANG_ATAS  = 2200; // AMBANG_BAWAH + 200 = 2200

const int PIN_BTN = 27;
const int PIN_LED = 4;
const int PIN_POT = 34;

bool modeAuto = false;
int outState = 0;
unsigned long lastReportMs = 0;

void cetakStatus() {
  int raw = analogRead(PIN_POT);
  int btn = (digitalRead(PIN_BTN) == LOW) ? 1 : 0;
  Serial.printf("MODE=%s RAW=%d BTN=%d OUT=%d\n",
                modeAuto ? "AUTO" : "MANUAL", raw, btn, outState);
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P14-F\n", NIM);

  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  analogReadResolution(12);
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

  if (millis() - lastReportMs >= 500) {
    lastReportMs = millis();
    cetakStatus();
  }
}