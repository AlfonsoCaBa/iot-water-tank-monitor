# Technical decisions

Initial record: November 18, 2026. A proposal is not a verified implementation.

## 001 — Arduino IDE 2 and the ESP32 core

**Status: selected.** We will use C++ with the environment taught in class. Espressif's core provides microcontroller support. We will record the versions actually used and start with built-in functions before adding libraries.

## 002 — Dashboard using familiar technologies

**Status: selected.** HTML, CSS, and JavaScript cover the planned indicators, web requests, and alerts. A charting library will be selected and justified in Stage 5 if useful. Dashboard hosting will also be decided then.

## 003 — HTTPS and managed storage

**Status: Firebase selected by the student on November 24, 2026; live verification pending.** Use **Firebase Realtime Database** for a JSON tree containing the latest sample and timestamped history. A dedicated Email/Password account obtains expiring ID tokens; rules restrict reads and validated writes to its UID. The ESP32 uses HTTPS REST and JSON support already present in core 4.0.0, without another Firebase library. See the [setup and data format](cloud.md) and [Firebase REST documentation](https://firebase.google.com/docs/database/rest/start).

## 004 — Container and conversion to liters

**Status: updated calibration accepted based on the student's report.** Use nine distance/volume anchors, **20.56 cm at empty**, **2.98 cm at 2 L**, and piecewise interpolation. Height and volume percentages remain distinct. Earlier inclined-setup references are historical; the student's later report does not establish a numerical accuracy bound. See the [current calibration](calibration.md) and [container history](container-measurements.md).

## 005 — Learning and progress control

**Status: selected by the student.** Explain before writing code, work on one stage at a time, and investigate errors before moving forward. The student creates commits. Advanced improvements are deferred until the basic system has been tested.

## 006 — English documentation and manual Git commands

**Status: selected by the student.** All repository documentation, diagram labels, code comments, and commit messages will be written in English. Each suggested commit will include the exact commands for reviewing changes, staging files, creating the commit, and pushing it. The student runs these commands; the assistant does not create commits or push automatically.

## 007 — Median filtering and explicit invalid data

**Status: implemented for Stage 2 validation.** Use the median of five consecutive usable samples to reduce isolated distance spikes without another library. Invalid readings reset the window and do not become zero liters. Small endpoint excursions are allowed within a provisional 0.5 cm margin, clamped, and visibly flagged; larger departures are invalid. This setting needs review against physical reading series. The processing code is shared with computer-side C++ tests; those tests do not replace sensor and volume validation.

## 008 — Separate raw collection from calibrated operation

**Status: selected on November 24, 2026.** Collect replacement measurements with a standalone `calibration_reader` sketch that does not apply the withdrawn table, endpoint margin, or smoothing. Preserve errors and raw samples. Update the volume program only after the new data arrive, then let the student validate its estimates with water. This prevents old calibration bounds from biasing new data collection.
