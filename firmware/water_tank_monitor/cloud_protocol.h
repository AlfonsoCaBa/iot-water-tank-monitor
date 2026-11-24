#pragma once

#include <cstdint>
#include <cstring>

namespace cloud {

const std::uint32_t UPLOAD_INTERVAL_MS = 20000;
const std::uint32_t MAX_SAMPLE_AGE_MS = 5000;
const std::uint32_t AUTH_RETRY_MS = 60000;

inline bool sampleIsFresh(std::uint32_t now, std::uint32_t sampledAt) {
  return now - sampledAt <= MAX_SAMPLE_AGE_MS;
}

inline bool uploadIntervalElapsed(std::uint32_t now, std::uint32_t lastAttempt,
                                  bool attemptedBefore) {
  return !attemptedBefore || now - lastAttempt >= UPLOAD_INTERVAL_MS;
}

// Firebase returns an empty HTTP 204 response when print=silent is requested.
inline bool entryWasStored(int httpStatus) {
  return httpStatus == 204;
}

inline bool identifierIsSafe(const char* value) {
  if (value == nullptr || value[0] == '\0') return false;
  for (const char* p = value; *p != '\0'; ++p) {
    if (!((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') ||
          (*p >= '0' && *p <= '9') || *p == '_' || *p == '-')) return false;
  }
  return true;
}

// Accept only a Firebase database root URL, never an arbitrary token recipient.
inline bool databaseUrlIsValid(const char* url) {
  if (url == nullptr || std::strncmp(url, "https://", 8) != 0) return false;
  const char* host = url + 8;
  std::size_t length = std::strlen(host);
  if (length > 0 && host[length - 1] == '/') --length;
  if (length == 0 || host[0] == '.' || host[0] == '-') return false;
  for (std::size_t i = 0; i < length; ++i) {
    const char c = host[i];
    if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
          c == '-' || c == '.')) return false;
  }
  const char* suffixes[] = {".firebaseio.com", ".firebasedatabase.app"};
  for (const char* suffix : suffixes) {
    const std::size_t suffixLength = std::strlen(suffix);
    if (length > suffixLength &&
        std::strncmp(host + length - suffixLength, suffix, suffixLength) == 0) {
      return true;
    }
  }
  return false;
}

}  // namespace cloud
