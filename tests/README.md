# Tests and evidence

**Status:** the user confirmed USB detection through a disconnect/reconnect check and reported the installed ESP32 core and selected board profile. A manual board-check sketch is available; compilation, upload, serial output, and sensor measurements remain unverified. There are no automated tests yet.

At each stage, we will record the setup, versions, procedure, expected result, observed result, and limitations. Failed trials must also be documented.

| Stage | Planned check |
| --- | --- |
| 0 | Open the IDE, confirm the core, detect the ESP32 over USB, and verify upload and repeated serial output with the board-check sketch. |
| 1 | Compare distances with a ruler and observe readings with no echo. |
| 2 | Check empty, full, and intermediate quantities, invalid calibration, and limits. |
| 3 | Disconnect and restore Wi-Fi. |
| 4 | Confirm storage and handle service failures. |
| 5 | Review indicators, history, alerts, and stale data. |
| 6 | Repeat the complete data flow with different amounts of water. |

Calculation tests do not replace physical sensor tests. We will add result files and software tests when implementations exist that justify them. No numerical example will be presented as an actual measurement.
