# ESP32 firmware

## Current sketch — Stage 4

Open [water_tank_monitor.ino](water_tank_monitor/water_tank_monitor.ino) in Arduino IDE with all its headers in the same folder. The sketch measures distance, validates it, takes a five-sample median, calculates volume, maintains Wi-Fi, and sends the latest valid result to Firebase in a background task.

| File | Responsibility |
| --- | --- |
| `water_tank_monitor.ino` | Sensor pins/timing, serial output, Wi-Fi connection and 15-second reconnection attempts. |
| `tank_processing.h` | Nine calibration anchors, validation, interpolation, percentages, and median filtering. |
| `cloud_upload.h` | NTP, verified HTTPS, Firebase authentication/token renewal, one-slot sample queue, and periodic storage. |
| `cloud_protocol.h` | Upload timing, sample freshness, database URL validation, and silent-write response acceptance. |
| `secrets.example.h` | Empty configuration template. Actual credentials belong in ignored `secrets.h`. |

Follow the [Firebase guide](../docs/cloud.md) to create the database, device account, and access rules. The existing local `secrets.h` has four Firebase placeholders; fill them without replacing the working Wi-Fi credentials. When cloning the repository, copy `secrets.example.h` to `secrets.h` first.

The installed ESP32 core **4.0.0** includes Wi-Fi, HTTP, TLS with its CA bundle, FreeRTOS, and `cJSON`. No additional library installation is required. Arduino compiles the sketch and includes its headers; do not compile each header separately.

## Hardware and calibration

- Board: **ESP32 Dev Module**; select its actual Windows COM port.
- Serial Monitor: **115200 baud**.
- TRIG: GPIO18. ECHO: GPIO19 through the [voltage divider](../docs/wiring.md).
- Measurements: at least 500 ms apart; ECHO timeout 30 ms.
- Distance: echo microseconds × 0.0343 / 2, accounting for the round trip.
- Current references: **20.56 cm → 0 L**, **2.98 cm → 2 L**, with seven intermediate anchors.
- A 0.5 cm endpoint allowance gives a usable raw range of **2.48–21.06 cm**. Small excursions are clamped and flagged; larger errors reset the filter.
- Startup or an error requires five usable readings before a new median result. See the [calibration guide](../docs/calibration.md).

Stage 2 volume behavior and Stage 3 hotspot recovery were accepted based on the student's reports. Quantitative physical error bounds have not been established. Stage 4 Arduino compilation and live Firebase storage remain pending.

## Earlier standalone tools

- [board_check.ino](board_check/board_check.ino): upload and uptime output only.
- [calibration_reader.ino](calibration_reader/calibration_reader.ino): raw distances without calibrated volume limits or filtering.

These tools replace the running program when uploaded. Earlier calibration records remain in `docs/` and `tests/results/`; the active processing header is the source for current anchors.
