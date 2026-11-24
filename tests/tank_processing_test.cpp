#include <cassert>
#include <cmath>
#include <iostream>

#include "../firmware/water_tank_monitor/tank_processing.h"

void expectNear(float actual, float expected) {
  assert(std::isfinite(actual));
  assert(std::fabs(actual - expected) < 0.0002f);
}

void expectInvalid(float distanceCm) {
  const tank::TankReading reading = tank::calculateReading(distanceCm);
  assert(!reading.valid);
  assert(std::isnan(reading.levelPercent));
  assert(std::isnan(reading.liters));
  assert(std::isnan(reading.fillPercent));
}

int main() {
  assert(tank::calibrationIsValid());

  // The measured anchors must reproduce their reported reference volumes.
  const float distances[] = {20.56f, 17.29f, 15.98f, 13.93f, 11.54f, 8.82f, 7.82f, 4.92f, 2.98f};
  const float expectedLiters[] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 1.75f, 2.0f};
  const float expectedFill[] = {0.0f, 12.5f, 25.0f, 37.5f, 50.0f, 62.5f, 75.0f, 87.5f, 100.0f};
  for (int i = 0; i < 9; ++i) {
    const tank::TankReading reading = tank::calculateReading(distances[i]);
    assert(reading.valid);
    assert(!reading.endpointClamped);
    expectNear(reading.liters, expectedLiters[i]);
    expectNear(reading.fillPercent, expectedFill[i]);
  }
  expectNear(tank::calculateReading(17.29f).levelPercent, 18.600683f);
  expectNear(tank::calculateReading(11.54f).levelPercent, 51.308305f);

  // Midpoints exercise every segment, not just table lookups.
  expectNear(tank::calculateReading(18.925f).liters, 0.125f);
  expectNear(tank::calculateReading(16.635f).liters, 0.375f);
  expectNear(tank::calculateReading(14.955f).liters, 0.625f);
  expectNear(tank::calculateReading(12.735f).liters, 0.875f);
  expectNear(tank::calculateReading(10.18f).liters, 1.125f);
  expectNear(tank::calculateReading(8.32f).liters, 1.375f);
  expectNear(tank::calculateReading(6.37f).liters, 1.625f);
  expectNear(tank::calculateReading(3.95f).liters, 1.875f);
  expectNear(tank::calculateReading(10.86f).liters, 1.0625f);

  // Small endpoint deviations clamp; gross errors do not become 0 or 100%.
  const tank::TankReading belowEmpty = tank::calculateReading(20.8f);
  assert(belowEmpty.valid && belowEmpty.endpointClamped);
  expectNear(belowEmpty.liters, 0.0f);
  expectNear(belowEmpty.levelPercent, 0.0f);
  const tank::TankReading aboveFull = tank::calculateReading(2.8f);
  assert(aboveFull.valid && aboveFull.endpointClamped);
  expectNear(aboveFull.liters, 2.0f);
  expectNear(aboveFull.levelPercent, 100.0f);
  assert(tank::calculateReading(2.48f).valid);
  assert(tank::calculateReading(21.06f).valid);
  expectInvalid(2.47f);
  expectInvalid(21.07f);
  expectInvalid(0.0f);
  expectInvalid(500.0f);
  expectInvalid(NAN);
  expectInvalid(INFINITY);

  // Reject invalid calibration rather than dividing by zero or inventing data.
  const tank::CalibrationPoint duplicateDistance[] = {
    {20.2f, 0.0f}, {20.2f, 2.0f}
  };
  const tank::CalibrationPoint decreasingVolume[] = {
    {20.2f, 0.0f}, {11.9f, 1.0f}, {3.3f, 0.5f}
  };
  const tank::CalibrationPoint invalidNumber[] = {
    {20.2f, 0.0f}, {NAN, 2.0f}
  };
  assert(!tank::calibrationIsValid(nullptr, 0));
  assert(!tank::calibrationIsValid(tank::CALIBRATION_POINTS, 1));
  assert(!tank::calibrationIsValid(duplicateDistance, 2));
  assert(!tank::calibrationIsValid(decreasingVolume, 3));
  assert(!tank::calibrationIsValid(invalidNumber, 2));
  assert(!tank::calculateReading(10.0f, duplicateDistance, 2).valid);

  // Volume must stay bounded and decrease monotonically as distance increases.
  float previousLiters = 2.0f;
  for (int i = 0; i <= 1000; ++i) {
    const float distanceCm = 2.98f + 17.58f * i / 1000.0f;
    const tank::TankReading reading = tank::calculateReading(distanceCm);
    assert(reading.valid);
    assert(reading.liters >= 0.0f && reading.liters <= 2.0f);
    assert(reading.levelPercent >= 0.0f && reading.levelPercent <= 100.0f);
    assert(reading.fillPercent >= 0.0f && reading.fillPercent <= 100.0f);
    assert(reading.liters <= previousLiters + 0.00001f);
    previousLiters = reading.liters;
  }

  // Warmup, a plausible isolated outlier, window rollover, and reset behavior.
  tank::MedianFilter filter;
  float filteredCm = NAN;
  const float noisySamples[] = {11.9f, 11.8f, 19.0f, 12.0f, 11.9f};
  for (int i = 0; i < 4; ++i) {
    assert(!tank::addMedianSample(filter, noisySamples[i], filteredCm));
    assert(std::isnan(filteredCm));
  }
  assert(tank::addMedianSample(filter, noisySamples[4], filteredCm));
  expectNear(filteredCm, 11.9f);

  tank::resetFilter(filter);
  const float rollingSamples[] = {5.0f, 3.0f, 20.0f, 7.0f, 4.0f};
  for (int i = 0; i < 5; ++i) {
    tank::addMedianSample(filter, rollingSamples[i], filteredCm);
  }
  expectNear(filteredCm, 5.0f);
  assert(tank::addMedianSample(filter, 6.0f, filteredCm));
  expectNear(filteredCm, 6.0f);
  assert(tank::addMedianSample(filter, 8.0f, filteredCm));
  expectNear(filteredCm, 7.0f);
  assert(!tank::addMedianSample(filter, NAN, filteredCm));
  assert(filter.count == 0 && std::isnan(filteredCm));
  assert(!tank::addMedianSample(filter, 11.9f, filteredCm));
  assert(filter.count == 1 && std::isnan(filteredCm));

  std::cout << "All tank-processing tests passed.\n";
  return 0;
}
