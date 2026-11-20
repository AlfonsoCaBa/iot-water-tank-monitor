// Stage 0: check firmware upload and serial output with only USB connected.
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 board check started");
}

void loop() {
  Serial.print("Uptime (s): ");
  // millis() returns elapsed milliseconds since the program started.
  Serial.println(millis() / 1000);
  delay(2000);
}
