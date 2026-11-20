# Stage 1 — Functional sensor distance test

- **Date:** November 20, 2026.
- **Operator and evidence source:** student; qualitative result reported in the project chat. The assistant did not perform or independently observe the physical test.
- **Sketch:** [water_tank_monitor.ino](../../firmware/water_tank_monitor/water_tank_monitor.ino), using TRIG GPIO18 and ECHO GPIO19 through the voltage divider.
- **Prior electrical observations:** the user reported a 5.17 V board supply and 3.2 V output in the constant-input divider test. These are separate measurements, not sensor ECHO peak measurements.

## Reported procedure and result

The user reported running the program successfully, changing the target distance, and verifying the measurements with a ruler. The user described the distance measurements as working correctly.

**Result:** functional pass based on the user's report. This supports moving to container calibration in Stage 2.

## Limits of the evidence

- Exact target distances, sensor readings, sample counts, and numeric errors were not supplied.
- No quantitative accuracy, repeatability, or stability threshold has been demonstrated.
- Timeout and out-of-range behavior have not been physically demonstrated.
- A final assembly photograph, the supply pin's printed label, and the sensor supply voltage under load were not supplied.
- This test does not establish correct readings above water or correct percentage/liter calculations.

Retain numeric reference/measurement pairs during calibration and final testing. Do not present invented observations as results.

## Next step

Identify the container and its internal geometry, then obtain fixed-mount empty/full references and known-volume measurements using the [calibration guide](../../docs/calibration.md).
