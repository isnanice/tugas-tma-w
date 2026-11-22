const char NIM[] = "21120124120020";

const int LED1_PIN = 4;   // Source (OUTPUT)
const int LED2_PIN = 16;  // Sink (OUTPUT)
const int LED3_PIN = 17;  // Open-Drain susunan source (OUTPUT_OPEN_DRAIN)
const int LED4_PIN = 18;  // Open-Drain susunan sink (OUTPUT_OPEN_DRAIN)

void setup() {
  Serial.begin(115200);
  Serial.printf("NIM %s | P10-C\n", NIM);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT_OPEN_DRAIN);
  pinMode(LED4_PIN, OUTPUT_OPEN_DRAIN);
}

void loop() {
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(LED3_PIN, HIGH);
  digitalWrite(LED4_PIN, HIGH);
  Serial.println("Semua pin ditulis HIGH. LED mana yang menyala?");
  delay(2000);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
  Serial.println("Semua pin ditulis LOW. LED mana yang menyala?");
  delay(2000);
}