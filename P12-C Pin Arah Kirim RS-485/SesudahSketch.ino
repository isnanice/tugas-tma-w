const char NIM[] = "21120124120020";

const char* PESAN = "NIM0020\n";
const bool TUNGGU_SELESAI = true;

const int PIN_DE  = 4;
const int PIN_TX2 = 17;
const int PIN_RX1 = 26;

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P12-C\n", NIM);

  pinMode(PIN_DE, OUTPUT);
  digitalWrite(PIN_DE, LOW);

  Serial1.begin(9600, SERIAL_8N1, PIN_RX1, -1);
  Serial2.begin(9600, SERIAL_8N1, -1, PIN_TX2);
  delay(200);
}

void loop() {
  while (Serial1.available()) Serial1.read();

  unsigned long t0 = micros();
  digitalWrite(PIN_DE, HIGH);
  Serial2.print(PESAN);
  if (TUNGGU_SELESAI) {
    Serial2.flush();
  }
  digitalWrite(PIN_DE, LOW);
  unsigned long lamaDeUs = micros() - t0;

  delay(20);

  String diterima = "";
  int jumlahByte = 0;
  while (Serial1.available()) {
    char c = (char)Serial1.read();
    jumlahByte++;
    if (c == '\n') {
      diterima += '|';
    } else {
      diterima += c;
    }
  }

  Serial.printf("DE HIGH %lu us | penerima dapat %d byte: \"%s\"\n",
                lamaDeUs, jumlahByte, diterima.c_str());
  delay(2000);
}