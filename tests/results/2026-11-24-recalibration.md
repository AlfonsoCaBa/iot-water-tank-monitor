# Stage 2 — Level-surface recalibration

**Report date:** November 24, 2026. The student reported that the previous measurement setup used a slightly inclined surface and requested this sequence: repeat sensor measurements, update the program from the new readings, then physically verify the volume estimates.

## Status of earlier evidence

The [November 21 references](2026-11-21-water-calibration.md) are retained as historical observations, but withdrawn for the new setup. No tilt angle or corrected measurements were supplied. Do not apply a guessed distance offset, reuse those readings, or interpret passing software tests as confirmation of their physical accuracy.

The sensor's prior functional ruler test and the working USB upload path remain recorded. This task repeats the water calibration, not the entire electronics setup. Wiring stays on TRIG GPIO18 and ECHO GPIO19 through the divider.

## Current collection tool

Use [calibration_reader.ino](../../firmware/calibration_reader/calibration_reader.ino) at 115200 baud. It prints sample numbers, echo duration, and raw distance, without the old volume table, endpoint margin, or median filter. Missing echoes and out-of-sensor-range values are explicitly marked. No new physical run of this reader has been reported.

The volume sketch retains the earlier processing implementation for later updating. Its `CALIBRATION_MEASUREMENTS_CURRENT` flag is false, so uploading it now reports that recalibration is required and does not produce old volume estimates. This source change does not alter a sketch already running on the board; upload the reader to replace that sketch.

## Replacement data to collect

Keep the container upright on a level, stable surface and the sensor fixed, with its faces horizontal and aimed vertically down. Do not move or tilt the setup between water levels. Keep the selected usable maximum of 2000 ml, subject to valid sensor clearance at that level.

| Total water volume | Ten consecutive raw readings | Volume-reference method | Status |
| --- | --- | --- | --- |
| Empty / 0 ml | Not supplied. | Record how the container was emptied. | Pending. |
| 250 ml | Not supplied. | Record jug/cylinder or container markings. | Pending. |
| 1000 ml | Not supplied. | Record the reference used. | Pending. |
| 2000 ml | Not supplied. | Record the reference used. | Pending. |

Preserve error lines and note whether the water was settled. The 1000 and 2000 ml entries are total volumes, not additional amounts poured at those steps. If filling cumulatively, add 250 ml, then 750 ml, then 1000 ml, measuring those quantities rather than guessing.

## Remaining sequence

1. Review the replacement raw series and derive representative reference distances; no replacement values have been invented.
2. Update the calibration table, related tests, documentation, and volume sketch readiness flag, then compile.
3. The student uploads that new volume sketch and checks reference levels and separate 500 ml / 1500 ml validation amounts. These validation amounts are not calibration inputs at this stage.

No replacement calibration or new physical volume validation has been completed yet.
