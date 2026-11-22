#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char NIM[] = "21120124120020";

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int BARIS[4] = {13, 14, 16, 17}; // R1 - R4
const int KOLOM[4] = {18, 19, 23, 25}; // C1 - C4

const char PETA[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

String isian = "";

char pindai() {
  for (int r = 0; r < 4; r++) {
    pinMode(BARIS[r], OUTPUT);
    digitalWrite(BARIS[r], LOW);
    delayMicroseconds(10);

    for (int c = 0; c < 4; c++) {
      if (digitalRead(KOLOM[c]) == LOW) {
        delay(20);
        while (digitalRead(KOLOM[c]) == LOW) {
          delay(5);
        }
        pinMode(BARIS[r], INPUT);
        return PETA[r][c];
      }
    }

    pinMode(BARIS[r], INPUT);
  }
  return '\0';
}

void tampilLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tekan keypad");
  lcd.setCursor(0, 1);
  lcd.print("Isian: ");
  lcd.print(isian);
}

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P10-D\n", NIM);

  for (int r = 0; r < 4; r++) {
    pinMode(BARIS[r], INPUT);
  }
  for (int c = 0; c < 4; c++) {
    pinMode(KOLOM[c], INPUT_PULLUP);
  }

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  tampilLCD();
}

void loop() {
  char k = pindai();
  if (k != '\0') {
    Serial.printf("Tombol: %c\n", k);

    if (k == '*') {
      isian = "";
      tampilLCD();
    } else if (k == '#') {
      Serial.printf("Dikirim: %s\n", isian.c_str());
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Dikirim:");
      lcd.setCursor(0, 1);
      lcd.print(isian);
      isian = "";
    } else {
      isian += k;
      tampilLCD();
    }
  }
}