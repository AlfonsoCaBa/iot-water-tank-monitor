# Stage 2 — Active level-surface calibration

**Physical status:** accepted on November 24 based on the student's report of successful updated volume output. No numerical post-update accuracy result was supplied; see the [acceptance record](../tests/results/2026-11-24-volume-validation.md). The checks below remain the procedure for later repeatability and error characterization.

**Latest revision: November 24, 2026.** After changing tables, the student supplied physical-volume labels and serial output. The nine pairs below replace the earlier calibration rather than applying a single offset. Use filtered distances because the volume algorithm consumes them; empty uses the reported raw 20.56 cm because the previous calibration window rejected that reading. The volume sketch is enabled. Matching these new anchors is a fit to the supplied data, not independent validation of accuracy.

| Reported water volume | Reported distance | Volume percentage of usable capacity |
| --- | --- | --- |
| 0 ml | 20.56 cm | 0% |
| 250 ml | 17.29 cm | 12.5% |
| 500 ml | 15.98 cm | 25% |
| 750 ml | 13.93 cm | 37.5% |
| 1000 ml | 11.54 cm | 50% |
| 1250 ml | 8.82 cm | 62.5% |
| 1500 ml | 7.82 cm | 75% |
| 1750 ml | 4.92 cm | 87.5% |
| 2000 ml | 2.98 cm | 100% |

These are student-reported representative values, not independently observed measurements. Raw series and the independent volume-reference method were not supplied.

For 750 ml, the reported raw distance was 13.91 cm and the filtered distance was 13.93 cm; the table deliberately uses 13.93 cm. The latest 19.24 cm distance to the lower narrowing is 1.32 cm above the new empty reference. It is geometry information, not an additional volume anchor.

## Calculations

The table is stored in [tank_processing.h](../firmware/water_tank_monitor/tank_processing.h). Height percentage uses `100 × (20.56 - distance) / (20.56 - 2.98)`, with a calibrated span of 17.58 cm. Liters are interpolated between neighboring entries; volume percentage is `100 × liters / 2.0`. Height and volume percentages remain distinct.

The five-reading median, explicit error handling, and bounded percentages are retained. The initial endpoint allowance remains 0.5 cm, making the accepted raw range **2.48–21.06 cm**, also subject to the sensor's 2–400 cm range. The new empty reference is now accepted. Small endpoint excursions are clamped and flagged; gross departures reset the filter and report an error. The allowance is provisional, not measured accuracy.

## Validation procedure

Open or reload [water_tank_monitor.ino](../firmware/water_tank_monitor/water_tank_monitor.ino), keeping its processing header in the same folder. The readiness flag is true. The student will compile/upload and open Serial Monitor at **115200 baud**, as requested. Let the water settle and allow about three seconds for the median window to refresh.

Check the nine reference levels again, then test unused quantities such as **600 or 1400 ml** for independent interpolation checks. All 250 ml increments through 2000 ml now define the table. Preserve actual output and errors and report the volume-reference method. Keep the mount and position unchanged. A phone reading of zero inclination alone does not establish identical sensor-to-container geometry or identical acoustic reflections; repeated changes at the same known volume require investigation rather than endless curve fitting.

Updated computer-side C++ calculation tests passed for all nine anchors and eight interpolation segments, invalid inputs/calibration, boundaries, monotonicity, and median behavior. This does not establish physical accuracy or repeatability. No new ESP32 compilation or upload was performed by the assistant for this update.
