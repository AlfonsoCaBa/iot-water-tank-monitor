# Tests and evidence

**Status:** Stages 0–2 are accepted based on student reports. The latest nine-point calibration passed computer-side calculation tests and the student reported successful updated volume output; see the [acceptance record](results/2026-11-24-volume-validation.md). Numerical post-update physical errors were not supplied. The earlier inclined-setup references remain historical; the [active calibration](../docs/calibration.md) is the current source of reference values.

## Computer-side processing tests

[tank_processing_test.cpp](tank_processing_test.cpp) includes the same [header](../firmware/water_tank_monitor/tank_processing.h) used by the ESP32. It tests the actual calculations and filter, without pretending to operate a sensor or duplicating the implementation in another language.

Coverage includes anchor reproduction, every interpolation segment, height versus volume percentage, endpoint clamping, rejection of invalid distances and calibration, bounded/monotonic volume over 1001 distances, and median warmup, isolated outliers, rolling-window order, and reset behavior.

On the current Windows computer, `g++` is already available. From the project directory in PowerShell, run one command at a time:

```powershell
New-Item -ItemType Directory -Force "tests/build" | Out-Null
g++ -std=c++11 -Wall -Wextra -Werror -pedantic "tests/tank_processing_test.cpp" -o "tests/build/tank_processing_test.exe"
.\tests\build\tank_processing_test.exe
```

If compilation fails, stop before running the executable so an older build is not mistaken for a new result. The expected successful output is `All tank-processing tests passed.` Build files are ignored by Git. No test framework or extra library is needed.

## Physical checks by stage

| Stage | Check |
| --- | --- |
| 0 | Open the IDE, confirm the core, detect the ESP32 over USB, and verify upload and repeated serial output. |
| 1 | Compare distances with a ruler and observe missing-echo behavior. Numeric accuracy and error-case observations still need to be retained. |
| 2 | Accepted based on the student's qualitative report. Retain quantitative output and errors at independent quantities such as 600 and 1400 ml when available. |
| 3 | Disconnect and restore Wi-Fi. |
| 4 | Confirm storage and handle service failures. |
| 5 | Review indicators, history, alerts, and stale data. |
| 6 | Repeat the complete data flow with different amounts of water. |

Record the setup, versions, procedure, expected result, observed result, and limitations. Failed trials also belong in the evidence. Synthetic test fixtures, expected output, and calibration anchors are not independent physical validation results.
