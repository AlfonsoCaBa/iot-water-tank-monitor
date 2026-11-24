#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>
#include <WiFi.h>
#include <cJSON.h>
#include <sys/time.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include "cloud_protocol.h"
#include "tank_processing.h"

namespace cloud {

struct Sample {
  bool valid;
  float distanceCm;
  float liters;
  float fillPercent;
  float levelPercent;
  bool endpointClamped;
  std::uint32_t sampledAtMs;
  std::int64_t measuredAt;
};

static QueueHandle_t pendingSample = nullptr;
static const char* apiKey = "";
static const char* deviceEmail = "";
static const char* devicePassword = "";
static String databaseUrl;
static String idToken;
static String deviceUid;
static std::uint32_t tokenIssuedMs = 0;
static std::uint32_t tokenLifetimeMs = 0;
// A plausibility floor for NTP, not a fixed current date.
const time_t MIN_VALID_UTC = 1704067200;  // January 1, 2024 UTC.

inline void invalidateReading() {
  if (pendingSample == nullptr) return;
  const Sample sample = {};
  xQueueOverwrite(pendingSample, &sample);
}

inline void publishReading(float distanceCm, const tank::TankReading& reading) {
  if (pendingSample == nullptr) return;
  if (!reading.valid || !std::isfinite(distanceCm)) {
    invalidateReading();
    return;
  }
  struct timeval now;
  gettimeofday(&now, nullptr);
  const std::int64_t measuredAt =
      static_cast<std::int64_t>(now.tv_sec) * 1000 + now.tv_usec / 1000;
  const Sample sample = {true, distanceCm, reading.liters, reading.fillPercent,
                         reading.levelPercent, reading.endpointClamped,
                         millis(), measuredAt};
  // A one-slot queue keeps the latest result without blocking the sensor.
  xQueueOverwrite(pendingSample, &sample);
}

inline int sendJson(const String& url, const char* method, const String& body,
                    String* response = nullptr) {
  NetworkClientSecure tls;
  // Built into the installed ESP32 core 4.0.0; verify Google's certificates.
  tls.useBuiltinCACertBundle();
  tls.setHandshakeTimeout(5);
  HTTPClient http;
  http.setConnectTimeout(3000);
  http.setTimeout(5000);
  if (!http.begin(tls, url)) return -1000;
  http.addHeader("Content-Type", "application/json");
  const int status = http.sendRequest(method, body);
  if (response != nullptr && status == 200) *response = http.getString();
  http.end();
  return status;
}

inline bool tokenIsUsable() {
  return !idToken.isEmpty() && millis() - tokenIssuedMs < tokenLifetimeMs;
}

inline bool authenticate() {
  // cJSON is included in this ESP32 core and safely escapes credentials.
  cJSON* request = cJSON_CreateObject();
  if (request == nullptr) return false;
  const bool complete =
      cJSON_AddStringToObject(request, "email", deviceEmail) != nullptr &&
      cJSON_AddStringToObject(request, "password", devicePassword) != nullptr &&
      cJSON_AddBoolToObject(request, "returnSecureToken", true) != nullptr;
  char* encoded = complete ? cJSON_PrintUnformatted(request) : nullptr;
  cJSON_Delete(request);
  if (encoded == nullptr) {
    Serial.println("Firebase: unable to allocate authentication JSON");
    return false;
  }
  String body(encoded);
  cJSON_free(encoded);
  const String url =
      String("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=") +
      apiKey;
  String response;
  const int status = sendJson(url, "POST", body, &response);
  if (status != 200) {
    Serial.printf("Firebase auth failed | HTTP: %d | Check API key, email/password and Email/Password provider; retry in 60 s\n",
                  status);
    return false;
  }

  cJSON* result = cJSON_Parse(response.c_str());
  const cJSON* token = cJSON_GetObjectItemCaseSensitive(result, "idToken");
  const cJSON* uid = cJSON_GetObjectItemCaseSensitive(result, "localId");
  const cJSON* expires = cJSON_GetObjectItemCaseSensitive(result, "expiresIn");
  bool valid = cJSON_IsString(token) && token->valuestring[0] != '\0' &&
               cJSON_IsString(uid) && identifierIsSafe(uid->valuestring) &&
               cJSON_IsString(expires);
  const long seconds = valid ? String(expires->valuestring).toInt() : 0;
  valid = valid && seconds > 60 && seconds <= 86400;
  if (valid) {
    idToken = token->valuestring;
    deviceUid = uid->valuestring;
    tokenIssuedMs = millis();
    // Sign in again before expiry; credentials and tokens are never logged.
    tokenLifetimeMs = static_cast<std::uint32_t>(seconds - 60) * 1000;
    Serial.printf("Firebase authenticated | Device UID: %s\n", deviceUid.c_str());
  } else {
    Serial.println("Firebase: invalid authentication response; retry in 60 s");
  }
  cJSON_Delete(result);
  return valid;
}

inline void uploadSample(const Sample& sample) {
  char record[384];
  const int length = snprintf(
      record, sizeof(record),
      "{\"distanceCm\":%.3f,\"liters\":%.4f,\"fillPercent\":%.3f,"
      "\"levelPercent\":%.3f,\"endpointClamped\":%s,"
      "\"measuredAt\":%lld,\"receivedAt\":{\".sv\":\"timestamp\"}}",
      sample.distanceCm, sample.liters, sample.fillPercent, sample.levelPercent,
      sample.endpointClamped ? "true" : "false",
      static_cast<long long>(sample.measuredAt));
  if (length < 0 || static_cast<std::size_t>(length) >= sizeof(record)) {
    Serial.println("Firebase: sample JSON exceeds buffer; upload skipped");
    return;
  }
  char entryId[24];
  snprintf(entryId, sizeof(entryId), "%lld",
           static_cast<long long>(sample.measuredAt));
  // One multi-path PATCH updates latest and appends a history record atomically.
  String body = String("{\"latest\":") + record + ",\"history/" + entryId + "\":" +
                record + "}";
  const String url = databaseUrl + "/devices/" + deviceUid +
                     ".json?auth=" + idToken + "&print=silent";
  const int status = sendJson(url, "PATCH", body);
  if (entryWasStored(status)) {
    Serial.printf("Cloud stored | Firebase history ID: %s\n", entryId);
  } else {
    if (status == 401) idToken = "";
    // Never print the URL, tokens, password, or arbitrary server responses.
    Serial.printf("Firebase upload failed | HTTP: %d | Check database URL and device UID in rules; next attempt uses a fresh reading\n",
                  status);
  }
}

inline void uploadTask(void*) {
  bool clockStarted = false;
  bool attemptedBefore = false;
  bool authAttempted = false;
  std::uint32_t lastAttemptMs = 0;
  std::uint32_t lastAuthAttemptMs = 0;
  std::uint32_t lastClockNoticeMs = 0;

  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(100));
    if (WiFi.status() != WL_CONNECTED) continue;
    if (!clockStarted) {
      configTime(0, 0, "pool.ntp.org", "time.google.com");
      clockStarted = true;
      lastClockNoticeMs = millis();
      Serial.println("Cloud: synchronizing UTC clock for HTTPS");
    }
    if (time(nullptr) < MIN_VALID_UTC) {
      if (millis() - lastClockNoticeMs >= UPLOAD_INTERVAL_MS) {
        Serial.println("Cloud: waiting for clock; check hotspot internet/NTP access");
        lastClockNoticeMs = millis();
      }
      continue;
    }
    if (!uploadIntervalElapsed(millis(), lastAttemptMs, attemptedBefore)) continue;
    if (!tokenIsUsable()) {
      if (authAttempted && millis() - lastAuthAttemptMs < AUTH_RETRY_MS) continue;
      const bool signedIn = authenticate();
      lastAuthAttemptMs = millis();
      authAttempted = true;
      if (!signedIn) continue;
    }

    // Select the latest sample AFTER any potentially slow authentication.
    Sample sample;
    if (xQueuePeek(pendingSample, &sample, 0) != pdTRUE || !sample.valid ||
        !sampleIsFresh(millis(), sample.sampledAtMs) ||
        sample.measuredAt < static_cast<std::int64_t>(MIN_VALID_UTC) * 1000) continue;
    uploadSample(sample);
    lastAttemptMs = millis();
    attemptedBefore = true;
  }
}

inline void begin(const char* key, const char* url, const char* email,
                  const char* password) {
  if (!identifierIsSafe(key) || email == nullptr || email[0] == '\0' ||
      password == nullptr || password[0] == '\0') {
    Serial.println("Firebase not configured: fill the four FIREBASE settings in secrets.h");
    return;
  }
  if (!databaseUrlIsValid(url)) {
    Serial.println("Firebase: use the HTTPS Realtime Database root URL in secrets.h");
    return;
  }
  apiKey = key;
  deviceEmail = email;
  devicePassword = password;
  databaseUrl = url;
  if (databaseUrl.endsWith("/")) databaseUrl.remove(databaseUrl.length() - 1);
  pendingSample = xQueueCreate(1, sizeof(Sample));
  if (pendingSample == nullptr) {
    Serial.println("Cloud: unable to allocate sample queue; measurements continue");
    return;
  }
  if (xTaskCreate(uploadTask, "cloud-upload", 16384, nullptr, 1, nullptr) != pdPASS) {
    vQueueDelete(pendingSample);
    pendingSample = nullptr;
    Serial.println("Cloud: unable to start upload task; measurements continue");
    return;
  }
  Serial.println("Firebase uploader ready | Interval: 20 s | HTTPS");
}

}  // namespace cloud
