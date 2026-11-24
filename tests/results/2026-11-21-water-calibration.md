# Stage 2 — Fixed-mount water calibration

**Historical record — withdrawn for reuse on November 24, 2026.** The student subsequently reported an inclined measurement surface and requested new readings on a level setup. Values below remain as the original report, not the current calibration. See the [replacement collection record](2026-11-24-recalibration.md). Passing calculation tests below does not establish the old measurements' physical accuracy.

**Report date:** November 21, 2026. **Evidence source:** measurements reported by the student in the project chat. The assistant did not perform or independently observe the water measurements. Their exact acquisition time, raw sample counts, and independent volume-reference method were not specified.

## Reported setup and active references

The student reported making mounting holes and fixing the sensor facing downward. The selected operating maximum is **2000 ml**, rather than the container's 2500 ml marking.

| Reported water volume | Reported sensor distance |
| --- | --- |
| Empty / 0 ml | 20.2 cm |
| 250 ml | 17.6 cm |
| 1000 ml | 11.9 cm |
| 2000 ml | 3.3 cm |

An additional reported distance of **19.2 cm** reaches the start of the lower narrowing. It corresponds to a 1.0 cm difference from the empty reference. No volume was supplied there; it is not used as an extra interpolation anchor.

These values supersede the preliminary 21.9 cm empty observation for the current mount. The 23 cm physical height and molded 2500 ml marking remain historical geometry observations, not active firmware constants.

## Implementation

The [processing header](../../firmware/water_tank_monitor/tank_processing.h) uses the four reported volume-distance pairs. Height percentage uses the 20.2 cm and 3.3 cm endpoints; liters use interpolation within the appropriate segment; volume percentage uses 2.0 L as 100%. A five-sample median and explicit invalid-reading handling are implemented. See the [calibration guide](../../docs/calibration.md).

## Software verification

The computer-side [C++ tests](../tank_processing_test.cpp) executed successfully and printed `All tank-processing tests passed.` They exercised anchor reproduction, intermediate segment values, bounded and monotonic outputs over 1001 distances, endpoint clamping, invalid inputs and calibration, and median warmup/outlier/window/reset behavior.

These tests execute the same processing header used by the firmware. Their input series are synthetic test fixtures; they are not physical sensor observations.

The final sketch also compiled successfully with Arduino CLI for `esp32:esp32:esp32` using core **4.0.0**. The compiler reported **294173 bytes** of program storage (22%) and **23240 bytes** of global-variable memory (7%). No upload or physical sensor test was performed by the assistant. Generated binaries and the local CLI configuration remain in ignored build directories.

## Physical validation still needed

- Upload the updated firmware and confirm reference-level output with the same mount.
- Test known **500 ml** and **1500 ml** amounts that were not used to define the table.
- Record output series, reference-volume method, absolute volume errors, endpoint behavior, and stability at the 3.3 cm full reference.
- Record sensor supply voltage under load, the final supply-pin label, and an updated assembly photograph when available; these were not supplied in earlier reports.

No independent accuracy or stability result is claimed. Stage 2 remains in progress.
