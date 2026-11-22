const char NIM[] = "21120124120020";

const int PIN_A = 27; // INPUT_PULLUP internal
const int PIN_B = 26; // INPUT_PULLUP internal (sudah diperbaiki)
const int PIN_C = 34; // INPUT_PULLUP (tetap memunculkan galat, ditarik R luar 10k ke 3V3)

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P09-C\n", NIM);

  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_C, INPUT_PULLUP);
}

void loop() {
  int a = digitalRead(PIN_A);
  int b = digitalRead(PIN_B);
  int c = digitalRead(PIN_C);

  Serial.printf("A(27, pull-up)=%d | B(26, tanpa pull)=%d | C(34, input-only)=%d\n", a, b, c);
  delay(500);
}