# ESP32 firmware

## Current task — run the updated volume sketch

The latest November 24 report supplies **nine** physical-volume/distance pairs, now applied with **20.56 cm at empty** and **2.98 cm at 2 L**. Open [water_tank_monitor.ino](water_tank_monitor/water_tank_monitor.ino) with the updated [tank_processing.h](water_tank_monitor/tank_processing.h) beside it. The readiness flag is true and computer-side calculation tests pass. Startup now prints `Calibration points: 9` to identify the update. ESP32 compilation/upload and physical validation are left to the student as requested. See [current calibration](../docs/calibration.md).

The earlier instructions below describe a previous calibration iteration. Its old distances, disabled flag, and old range are superseded. The current endpoint allowance is 0.5 cm, giving a usable raw range of **2.48–21.06 cm**. Keep the five-reading median and 115200 baud. All 250 ml increments now define anchors; use new quantities such as **600 or 1400 ml** for independent interpolation checks.

## Earlier collection and processing notes — historical

On November 24, the student reported an inclined measurement surface. Use [calibration_reader/calibration_reader.ino](calibration_reader/calibration_reader.ino) to collect new distances with the container upright on a level surface and the sensor fixed vertically down. The reader uses TRIG GPIO18, ECHO GPIO19, and Serial Monitor at 115200 baud, without a volume table, calibration-dependent limits, or smoothing.

Collect ten consecutive lines at each total volume: **0, 250, 1000, and 2000 ml**. Preserve errors and record the volume-reference method. See the [collection procedure](../docs/calibration.md) and [new record](../tests/results/2026-11-24-recalibration.md). Updated volume calculations and 500/1500 ml physical validation follow after these readings are reviewed.

## Volume sketch — waiting for new references

Open [water_tank_monitor/water_tank_monitor.ino](water_tank_monitor/water_tank_monitor.ino) in Arduino IDE. Keep [tank_processing.h](water_tank_monitor/tank_processing.h) in the same directory; Arduino IDE normally shows it in another tab. Reopen or reload the sketch if the editor still shows the older distance-only version.

The `.ino` controls the sensor and serial output. The header contains the calibration table, validation, formulas, and median filter so those calculations can also be tested on the computer. No third-party libraries, Wi-Fi, or cloud code are included.

### Reading the processing header

| Part | Purpose |
| --- | --- |
| `CalibrationPoint` and `CALIBRATION_POINTS` | A small array of distance/liter pairs; these are the user's measurements. |
| `calibrationIsValid()` | Checks the ordering and numeric values before any division or interpolation. |
| `distanceIsUsable()` | Rejects invalid or implausible distances before they can enter the filter. |
| `calculateReading()` | Produces height percentage, liters, and volume percentage from one filtered distance. |
| `TankReading` | Groups those results with a `valid` flag and an endpoint-clamping flag. Invalid numeric results are `NAN` (not a number), never a fabricated empty reading. |
| `MedianFilter` and `addMedianSample()` | Store five recent samples and return their middle value once the window is full. The function returns whether a median is ready and writes it into `filteredCm`. |
| `resetFilter()` | Requires five fresh samples after an error. |

The filter's index wraps around its five slots to replace the oldest reading. Sorting a separate copy preserves that order. The header uses standard C++ math functions and fixed-size arrays, without heap allocation. Function definitions are `inline` so this small header can be shared by the sketch and tests without a separate library build.

## Sensor and IDE settings

- TRIG: D18 / GPIO18, an output.
- ECHO: D19 / GPIO19 through the [voltage divider](../docs/wiring.md), an input.
- Board profile: **ESP32 Dev Module**.
- Current local ESP32 core confirmed during compilation: **4.0.0**.
- Port: select the actual Windows COM port; COM7/COM8 were previously identified as Bluetooth ports.
- Serial Monitor: **115200 baud**.

The trigger pulse lasts 10 microseconds. `pulseIn()` measures the ECHO high pulse with a 30000 microsecond timeout. Distance uses `echo_time × 0.0343 / 2`, approximating the speed of sound near room temperature and accounting for the round trip. Requests are at least 500 milliseconds apart. Sources: [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf), [Arduino pulseIn reference](https://docs.arduino.cc/language-reference/en/functions/advanced-io/pulseIn/).

## Calibration and processing

The retained historical table is **20.2 cm → 0 L**, **17.6 cm → 0.250 L**, **11.9 cm → 1.000 L**, and **3.3 cm → 2.000 L**, reported on November 21 and withdrawn for reuse on November 24. The old 19.2 cm lower-feature observation is not a volume point. Do not reuse these values for the new level setup.

`CALIBRATION_MEASUREMENTS_CURRENT` is currently false in the volume sketch. Uploading it reports `RECALIBRATION REQUIRED` and does not calculate old volumes. After receiving new readings, replace the table and related tests and enable the flag. This does not reprogram a sketch already running on the ESP32; upload the separate reader for data collection now.

The processing pipeline is:

1. Measure distance and reject pulse timeouts, invalid calibration, and unusable distances.
2. Collect five consecutive usable readings and take the median of the most recent five.
3. Calculate height percentage using the empty/full references.
4. Interpolate liters between the surrounding measured points and calculate fill percentage relative to 2 L.

`Level (% height)` and `Fill (% volume)` represent different quantities. At exactly 17.6 cm they are approximately 15.4% and 12.5%. See the [calibration guide](../docs/calibration.md) for formulas and an interpolation example.

The earlier processing configuration used a provisional 0.5 cm endpoint margin and a 2.8–20.7 cm usable range from the withdrawn references. It must not restrict replacement data collection; the separate reader only flags the sensor's specified 2–400 cm range. Revisit the processing allowance after reviewing the new series. Invalid readings reset the volume sketch's filter rather than reusing old measurements.

At startup or after an error, expect four `WARMUP` lines before the fifth usable sample produces a calculated output. Let the surface settle and allow approximately three seconds for the window to refresh after changing water levels. A mathematical example with a filtered distance of exactly 11.9 cm would produce:

```text
Raw (cm): 11.90 | Filtered (cm): 11.90 | Level (% height): 49.1 | Volume (L): 1.000 | Fill (% volume): 50.0
```

This is expected output, not an observed run of the updated firmware. Three decimal places in liters are display precision, not demonstrated accuracy.

## Validation status and next physical check

The Stage 1 distance-only sketch passed a user-reported ruler comparison; see the [earlier test record](../tests/results/2026-11-20-sensor-distance.md). Calculation tests passed for the historical fixture, but the water references have since been withdrawn. See the [historical Stage 2 record](../tests/results/2026-11-21-water-calibration.md) and [new collection record](../tests/results/2026-11-24-recalibration.md).

First collect the replacement raw readings. Once the volume sketch has been updated from them, verify/upload that new version and check **500 ml** and **1500 ml** validation amounts with the mount unchanged. Do not proceed to Wi-Fi until those results are reviewed.

## Earlier Stage 0 board check

[board_check/board_check.ino](board_check/board_check.ino) only checks upload and serial output. The student reported uptime values 1, 3, and 5 seconds; see its [test record](../tests/results/2026-11-20-board-check.md). This older sketch does not trigger the sensor or calculate volume.
