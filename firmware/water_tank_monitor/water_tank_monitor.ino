// Stage 2: calibrated water level and volume. No network code yet.
#include "tank_processing.h"

const int TRIG_PIN = 18;
const int ECHO_PIN = 19;
const unsigned long ECHO_TIMEOUT_US = 30000;
const float SOUND_CM_PER_US = 0.0343f;
const int SAMPLE_INTERVAL_MS = 500;
// New references were collected on a level surface with the sensor fixed.
const bool CALIBRATION_MEASUREMENTS_CURRENT = true;

tank::MedianFilter distanceFilter;
bool calibrationReady = false;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  delay(1000);
  calibrationReady = CALIBRATION_MEASUREMENTS_CURRENT && tank::calibrationIsValid();
  if (!CALIBRATION_MEASUREMENTS_CURRENT) {
    Serial.println("RECALIBRATION REQUIRED: use calibration_reader for new references");
  } else if (calibrationReady) {
    Serial.print("Water tank monitor started | Usable capacity (L): ");
    Serial.println(tank::CALIBRATION_POINTS[tank::CALIBRATION_POINT_COUNT - 1].liters, 3);
    Serial.print("Calibration points: ");
    Serial.println(tank::CALIBRATION_POINT_COUNT);
  } else {
    Serial.println("ERROR: invalid calibration table; measurements disabled");
  }
}

void loop() {
  if (!calibrationReady) {
    delay(SAMPLE_INTERVAL_MS);
    return;
  }

  // Request a measurement with a 10-microsecond trigger pulse.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoTimeUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (echoTimeUs == 0) {
    tank::resetFilter(distanceFilter);
    Serial.println("ERROR: no complete echo within 30 ms");
  } else {
    // Sound travels to the target and back, so divide by two.
    const float distanceCm = echoTimeUs * SOUND_CM_PER_US / 2.0f;

    if (!tank::distanceIsUsable(distanceCm)) {
      tank::resetFilter(distanceFilter);
      Serial.print("ERROR: distance outside usable calibration range | Raw (cm): ");
      Serial.println(distanceCm, 2);
    } else {
      float filteredCm;
      if (!tank::addMedianSample(distanceFilter, distanceCm, filteredCm)) {
        Serial.print("WARMUP: ");
        Serial.print(distanceFilter.count);
        Serial.print("/5 | Raw (cm): ");
        Serial.println(distanceCm, 2);
      } else {
        const tank::TankReading reading = tank::calculateReading(filteredCm);
        if (!reading.valid) {
          tank::resetFilter(distanceFilter);
          Serial.println("ERROR: calibration calculation failed");
        } else {
          Serial.print("Raw (cm): ");
          Serial.print(distanceCm, 2);
          Serial.print(" | Filtered (cm): ");
          Serial.print(filteredCm, 2);
          Serial.print(" | Level (% height): ");
          Serial.print(reading.levelPercent, 1);
          Serial.print(" | Volume (L): ");
          Serial.print(reading.liters, 3);
          Serial.print(" | Fill (% volume): ");
          Serial.print(reading.fillPercent, 1);
          if (reading.endpointClamped) Serial.print(" | Boundary: CLAMPED");
          Serial.println();
        }
      }
    }
  }

  // Space measurements to avoid interference from previous echoes.
  delay(SAMPLE_INTERVAL_MS);
}
