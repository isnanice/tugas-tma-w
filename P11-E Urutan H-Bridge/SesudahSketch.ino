const char NIM[] = "21120124120020";

const bool URUTAN_AMAN = false;

const int PIN_Q1 = 16;
const int PIN_Q2 = 17;
const int PIN_Q3 = 18;
const int PIN_Q4 = 19;

int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

void cekBahaya() {
  if (q1 == 1 && q2 == 1) {
    Serial.println("  BAHAYA: Q1 dan Q2 tersambung bersamaan!");
  }
  if (q3 == 1 && q4 == 1) {
    Serial.println("  BAHAYA: Q3 dan Q4 tersambung bersamaan!");
  }
}

void aturSaklar(int n1, int n2, int n3, int n4) {
  if (URUTAN_AMAN) {
    q1 = 0; q2 = 0; q3 = 0; q4 = 0;
    digitalWrite(PIN_Q1, LOW);
    digitalWrite(PIN_Q2, LOW);
    digitalWrite(PIN_Q3, LOW);
    digitalWrite(PIN_Q4, LOW);
    delay(500);

    q1 = n1; digitalWrite(PIN_Q1, q1); cekBahaya();
    q2 = n2; digitalWrite(PIN_Q2, q2); cekBahaya();
    q3 = n3; digitalWrite(PIN_Q3, q3); cekBahaya();
    q4 = n4; digitalWrite(PIN_Q4, q4); cekBahaya();
  } else {
    if (n1 == 1) { q1 = 1; digitalWrite(PIN_Q1, HIGH); cekBahaya(); }
    if (n2 == 1) { q2 = 1; digitalWrite(PIN_Q2, HIGH); cekBahaya(); }
    if (n3 == 1) { q3 = 1; digitalWrite(PIN_Q3, HIGH); cekBahaya(); }
    if (n4 == 1) { q4 = 1; digitalWrite(PIN_Q4, HIGH); cekBahaya(); }

    if (n1 == 0) { q1 = 0; digitalWrite(PIN_Q1, LOW); cekBahaya(); }
    if (n2 == 0) { q2 = 0; digitalWrite(PIN_Q2, LOW); cekBahaya(); }
    if (n3 == 0) { q3 = 0; digitalWrite(PIN_Q3, LOW); cekBahaya(); }
    if (n4 == 0) { q4 = 0; digitalWrite(PIN_Q4, LOW); cekBahaya(); }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P11-E\n", NIM);

  pinMode(PIN_Q1, OUTPUT);
  pinMode(PIN_Q2, OUTPUT);
  pinMode(PIN_Q3, OUTPUT);
  pinMode(PIN_Q4, OUTPUT);

  Serial.println("Ketik M, R, atau S.");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = toupper(Serial.read());
    if (cmd == 'M') {
      aturSaklar(1, 0, 0, 1);
      Serial.printf("Maju: Q1=%d Q2=%d Q3=%d Q4=%d\n", q1, q2, q3, q4);
    } else if (cmd == 'R') {
      aturSaklar(0, 1, 1, 0);
      Serial.printf("Mundur: Q1=%d Q2=%d Q3=%d Q4=%d\n", q1, q2, q3, q4);
    } else if (cmd == 'S') {
      aturSaklar(0, 0, 0, 0);
      Serial.printf("Berhenti: Q1=%d Q2=%d Q3=%d Q4=%d\n", q1, q2, q3, q4);
    }
  }
}