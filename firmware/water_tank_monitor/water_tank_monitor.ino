// Stage 4: calibrated measurements, Wi-Fi, and periodic cloud storage.
#include <WiFi.h>
#include "tank_processing.h"
#include "cloud_upload.h"

#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif

const int TRIG_PIN = 18;
const int ECHO_PIN = 19;
const unsigned long ECHO_TIMEOUT_US = 30000;
const float SOUND_CM_PER_US = 0.0343f;
const int SAMPLE_INTERVAL_MS = 500;
const unsigned long WIFI_RETRY_MS = 15000;
// New references were collected on a level surface with the sensor fixed.
const bool CALIBRATION_MEASUREMENTS_CURRENT = true;

tank::MedianFilter distanceFilter;
bool calibrationReady = false;
bool wifiConfigured = false;
bool wifiWasConnected = false;
unsigned long lastWifiAttemptMs = 0;

void startWiFi() {
  if (WIFI_SSID[0] == '\0' || WIFI_PASSWORD[0] == '\0') {
    Serial.println("Wi-Fi not configured: fill in secrets.h; measurements continue");
    return;
  }

  wifiConfigured = true;
  WiFi.mode(WIFI_STA);
  // Own the retry timing in loop(), rather than combining two retry mechanisms.
  WiFi.setAutoReconnect(false);
  Serial.println("Wi-Fi: connecting to hotspot; measurements continue");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lastWifiAttemptMs = millis();
}

void maintainWiFi() {
  if (!wifiConfigured) return;

  const bool connected = WiFi.status() == WL_CONNECTED;
  if (connected) {
    if (!wifiWasConnected) {
      Serial.print("Wi-Fi connected | IP: ");
      Serial.println(WiFi.localIP());
    }
    wifiWasConnected = true;
    return;
  }

  if (wifiWasConnected) {
    Serial.println("Wi-Fi disconnected; measurements continue");
  }
  wifiWasConnected = false;

  // Unsigned subtraction also works across the millis() counter rollover.
  if (millis() - lastWifiAttemptMs >= WIFI_RETRY_MS) {
    Serial.println("Wi-Fi: retrying hotspot connection");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    lastWifiAttemptMs = millis();
  }
}

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
  startWiFi();
  cloud::begin(FIREBASE_API_KEY, FIREBASE_DATABASE_URL,
               FIREBASE_DEVICE_EMAIL, FIREBASE_DEVICE_PASSWORD);
}

void loop() {
  maintainWiFi();
  if (!calibrationReady) {
    cloud::invalidateReading();
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
    cloud::invalidateReading();
    tank::resetFilter(distanceFilter);
    Serial.println("ERROR: no complete echo within 30 ms");
  } else {
    // Sound travels to the target and back, so divide by two.
    const float distanceCm = echoTimeUs * SOUND_CM_PER_US / 2.0f;

    if (!tank::distanceIsUsable(distanceCm)) {
      cloud::invalidateReading();
      tank::resetFilter(distanceFilter);
      Serial.print("ERROR: distance outside usable calibration range | Raw (cm): ");
      Serial.println(distanceCm, 2);
    } else {
      float filteredCm;
      if (!tank::addMedianSample(distanceFilter, distanceCm, filteredCm)) {
        cloud::invalidateReading();
        Serial.print("WARMUP: ");
        Serial.print(distanceFilter.count);
        Serial.print("/5 | Raw (cm): ");
        Serial.println(distanceCm, 2);
      } else {
        const tank::TankReading reading = tank::calculateReading(filteredCm);
        if (!reading.valid) {
          cloud::invalidateReading();
          tank::resetFilter(distanceFilter);
          Serial.println("ERROR: calibration calculation failed");
        } else {
          cloud::publishReading(filteredCm, reading);
          // One formatted write keeps this line together with cloud-task logs.
          Serial.printf("Raw (cm): %.2f | Filtered (cm): %.2f | Level (%% height): %.1f"
                        " | Volume (L): %.3f | Fill (%% volume): %.1f%s\n",
                        distanceCm, filteredCm, reading.levelPercent,
                        reading.liters, reading.fillPercent,
                        reading.endpointClamped ? " | Boundary: CLAMPED" : "");
        }
      }
    }
  }

  // Space measurements to avoid interference from previous echoes.
  delay(SAMPLE_INTERVAL_MS);
}
