# IoT Water Tank Monitor

University IoT project that estimates the water available in a small container representing a water storage tank, stores measurements in the cloud, and displays them on a web dashboard.

**Status: Stages 0 and 1 complete based on the user's functional test reports; Stage 2 — container calibration.** The user reported running the sensor sketch and checking distances with a ruler; see the [sensor test record](tests/results/2026-11-20-sensor-distance.md). [Container measurements and photographs](docs/container-measurements.md) are recorded. Fixed-mount water references, filtering, and conversion to liters remain pending; see the [calibration guide](docs/calibration.md). Dashboard implementation has not started.

## How it will work

The HC-SR04 will measure the gap between the sensor and the water surface. As the water rises, this distance decreases. The ESP32 will validate readings, calculate the water level and estimated liters, and send measurements over Wi-Fi to a storage service. The browser will retrieve the measurement history.

| IoT stage | Responsibility |
| --- | --- |
| Measurement | Measure distance with the HC-SR04. |
| Microprocessor | Validate, filter, and calculate with the ESP32. |
| Connection | Connect over Wi-Fi and send data using HTTPS. |
| Storage | Store measurements with timestamps in the cloud. |
| Visualization | Display the level, liters, history, and alerts on a web page. |

Height percentage and volume percentage only match when the container has a constant internal cross-sectional area. Usable capacity and geometry will be checked in Stage 2.

## Selected and proposed tools

- **Firmware:** C++ with Arduino IDE 2 and Espressif's ESP32 core.
- **Dashboard:** HTML, CSS, and JavaScript, edited in VS Code.
- **Version control:** Git and GitHub; the student creates commits.
- **Proposed cloud platform:** ThingSpeak for measurement history storage and its web API. The choice will be finalized in Stage 4 based on the course's database requirements. See the [technical decisions](docs/decisions.md).

## Repository structure

```text
iot-water-tank-monitor/
├── README.md
├── .gitignore
├── firmware/
│   ├── README.md
│   ├── board_check/
│   │   └── board_check.ino
│   └── water_tank_monitor/
│       └── water_tank_monitor.ino
├── dashboard/
│   └── README.md
├── docs/
│   ├── setup.md
│   ├── hardware.md
│   ├── wiring.md
│   ├── calibration.md
│   ├── container-measurements.md
│   ├── images/container/
│   ├── architecture.md
│   ├── decisions.md
│   └── roadmap.md
└── tests/
    └── README.md
```

Project directories start with documentation; code is added as we study each stage. The board-check sketch only verifies the development setup.

## Getting started

1. Review the [environment setup](docs/setup.md).
2. Confirm the board model and gather the [materials](docs/hardware.md).
3. Study the [conceptual wiring diagram](docs/wiring.md) before connecting components.
4. Follow the [development roadmap](docs/roadmap.md), checking each result before moving forward.

The [architecture](docs/architecture.md) describes the data flow. The [test plan](tests/README.md) explains how to record actual evidence.

## Working approach

- Work on one stage at a time; explain before writing code or making significant configuration changes.
- Keep repository documentation, code comments, and commit messages in English.
- Use small, descriptive commits, created manually by the student. Provide the exact review, staging, commit, and push commands for the student to run.
- Document components, calibration, decisions, and results as they become available.
- Keep Wi-Fi passwords and private keys out of the repository.
- Always distinguish expected results from verified observations.

Advanced improvements will be considered after the original scope has been implemented and tested.
