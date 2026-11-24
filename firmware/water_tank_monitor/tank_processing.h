#pragma once

#include <cmath>

// Pure calculations: shared by the ESP32 sketch and the computer-side tests.
namespace tank {

struct CalibrationPoint {
  float distanceCm;
  float liters;
};

// Latest physical-volume / filtered-distance pairs reported November 24, 2026.
// Use the raw 20.56 cm observation for empty; the old range rejected that value.
// Distances decrease as volume increases. Full means the usable 2 L limit.
const CalibrationPoint CALIBRATION_POINTS[] = {
  {20.56f, 0.0f},
  {17.29f, 0.25f},
  {15.98f, 0.5f},
  {13.93f, 0.75f},
  {11.54f, 1.0f},
  {8.82f, 1.25f},
  {7.82f, 1.5f},
  {4.92f, 1.75f},
  {2.98f, 2.0f}
};
const int CALIBRATION_POINT_COUNT =
    sizeof(CALIBRATION_POINTS) / sizeof(CALIBRATION_POINTS[0]);
const float SENSOR_MIN_CM = 2.0f;
const float SENSOR_MAX_CM = 400.0f;
// Initial allowance for endpoint noise, not a measured accuracy guarantee.
const float ENDPOINT_MARGIN_CM = 0.5f;
const int FILTER_SIZE = 5;

struct TankReading {
  bool valid;
  bool endpointClamped;
  float levelPercent;
  float liters;
  float fillPercent;
};

inline bool calibrationIsValid(
    const CalibrationPoint* points = CALIBRATION_POINTS,
    int count = CALIBRATION_POINT_COUNT) {
  if (points == nullptr || count < 2) return false;

  for (int i = 0; i < count; ++i) {
    if (!std::isfinite(points[i].distanceCm) ||
        !std::isfinite(points[i].liters) ||
        points[i].distanceCm < SENSOR_MIN_CM ||
        points[i].distanceCm > SENSOR_MAX_CM || points[i].liters < 0.0f) {
      return false;
    }
    if (i > 0 && (points[i].distanceCm >= points[i - 1].distanceCm ||
                  points[i].liters <= points[i - 1].liters)) {
      return false;
    }
  }
  return points[0].liters == 0.0f && points[count - 1].liters > 0.0f;
}

inline bool distanceIsUsable(
    float distanceCm, const CalibrationPoint* points = CALIBRATION_POINTS,
    int count = CALIBRATION_POINT_COUNT) {
  if (!calibrationIsValid(points, count) || !std::isfinite(distanceCm)) {
    return false;
  }
  return distanceCm >= SENSOR_MIN_CM && distanceCm <= SENSOR_MAX_CM &&
         distanceCm >= points[count - 1].distanceCm - ENDPOINT_MARGIN_CM &&
         distanceCm <= points[0].distanceCm + ENDPOINT_MARGIN_CM;
}

inline float clampValue(float value, float minimum, float maximum) {
  if (value < minimum) return minimum;
  if (value > maximum) return maximum;
  return value;
}

inline TankReading calculateReading(
    float distanceCm, const CalibrationPoint* points = CALIBRATION_POINTS,
    int count = CALIBRATION_POINT_COUNT) {
  // Invalid measurements must not look like an empty or full container.
  TankReading reading = {false, false, NAN, NAN, NAN};
  if (!distanceIsUsable(distanceCm, points, count)) return reading;

  const float emptyCm = points[0].distanceCm;
  const float fullCm = points[count - 1].distanceCm;
  const float capacityLiters = points[count - 1].liters;
  const float boundedCm = clampValue(distanceCm, fullCm, emptyCm);

  // Interpolate only within the two points surrounding this distance.
  float liters = 0.0f;
  for (int i = 0; i < count - 1; ++i) {
    if (boundedCm >= points[i + 1].distanceCm) {
      const float fraction = (points[i].distanceCm - boundedCm) /
          (points[i].distanceCm - points[i + 1].distanceCm);
      liters = points[i].liters + fraction *
          (points[i + 1].liters - points[i].liters);
      break;
    }
  }

  reading.valid = true;
  reading.endpointClamped = boundedCm != distanceCm;
  reading.levelPercent = clampValue(
      100.0f * (emptyCm - boundedCm) / (emptyCm - fullCm), 0.0f, 100.0f);
  reading.liters = clampValue(liters, 0.0f, capacityLiters);
  reading.fillPercent = clampValue(
      100.0f * reading.liters / capacityLiters, 0.0f, 100.0f);
  return reading;
}

struct MedianFilter {
  float samples[FILTER_SIZE] = {};
  int count = 0;
  int nextIndex = 0;
};

inline void resetFilter(MedianFilter& filter) {
  filter.count = 0;
  filter.nextIndex = 0;
}

inline bool addMedianSample(MedianFilter& filter, float sample,
                           float& filteredCm) {
  filteredCm = NAN;
  if (!std::isfinite(sample)) {
    resetFilter(filter);
    return false;
  }

  // Replace the oldest sample once the five-slot buffer is full.
  filter.samples[filter.nextIndex] = sample;
  filter.nextIndex = (filter.nextIndex + 1) % FILTER_SIZE;
  if (filter.count < FILTER_SIZE) ++filter.count;
  if (filter.count < FILTER_SIZE) return false;

  // Sort a copy so the original buffer keeps its time order.
  float sorted[FILTER_SIZE];
  for (int i = 0; i < FILTER_SIZE; ++i) sorted[i] = filter.samples[i];
  for (int i = 1; i < FILTER_SIZE; ++i) {
    const float value = sorted[i];
    int j = i;
    while (j > 0 && sorted[j - 1] > value) {
      sorted[j] = sorted[j - 1];
      --j;
    }
    sorted[j] = value;
  }

  filteredCm = sorted[FILTER_SIZE / 2];
  return true;
}

}  // namespace tank
