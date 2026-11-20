// Stage 1: measure distance only. No volume, filtering, or network code yet.
const int TRIG_PIN = 18;
const int ECHO_PIN = 19;
const unsigned long ECHO_TIMEOUT_US = 30000;
const float SOUND_CM_PER_US = 0.0343f;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  delay(1000);
  Serial.println("HC-SR04 distance test started");
}

void loop() {
  // Request a measurement with a 10-microsecond trigger pulse.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoTimeUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (echoTimeUs == 0) {
    Serial.println("ERROR: no complete echo within 30 ms");
  } else {
    // Sound travels to the target and back, so divide by two.
    const float distanceCm = echoTimeUs * SOUND_CM_PER_US / 2.0f;

    if (distanceCm < 2.0f || distanceCm > 400.0f) {
      Serial.println("ERROR: distance outside the specified 2-400 cm range");
    } else {
      Serial.print("Echo (us): ");
      Serial.print(echoTimeUs);
      Serial.print(" | Distance (cm): ");
      Serial.println(distanceCm, 1);
    }
  }

  // Space measurements to avoid interference from previous echoes.
  delay(500);
}
