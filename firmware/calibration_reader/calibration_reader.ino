// Collect raw distances without a volume table, endpoint limits, or filtering.
const int TRIG_PIN = 18;
const int ECHO_PIN = 19;
const unsigned long ECHO_TIMEOUT_US = 30000;
const float SOUND_CM_PER_US = 0.0343f;
unsigned long sampleNumber = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  delay(1000);
  Serial.println("Raw calibration reader | Record the water volume separately");
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoTimeUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  Serial.print("Sample: ");
  Serial.print(++sampleNumber);

  if (echoTimeUs == 0) {
    Serial.println(" | ERROR: no complete echo within 30 ms");
  } else {
    const float distanceCm = echoTimeUs * SOUND_CM_PER_US / 2.0f;
    Serial.print(" | Echo (us): ");
    Serial.print(echoTimeUs);
    Serial.print(" | Distance (cm): ");
    Serial.print(distanceCm, 2);
    if (distanceCm < 2.0f || distanceCm > 400.0f) {
      Serial.println(" | Status: OUT_OF_SENSOR_RANGE");
    } else {
      Serial.println(" | Status: OK");
    }
  }

  delay(500);
}
