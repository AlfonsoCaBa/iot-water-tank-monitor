# Development roadmap

**Active stage: 2 — container calibration.** Stages 0 and 1 are complete based on the user's functional test reports. Quantitative sensor accuracy and stability have not been established; see the recorded limitations in the [sensor test record](../tests/results/2026-11-20-sensor-distance.md). Each program's purpose, connections, and important parts will be explained before implementation.

| Stage | Work | Evidence needed to move forward | Status |
| --- | --- | --- | --- |
| 0 — Preparation | Review tools, identify the board, and organize the repository. | Working upload path and repeated board-check serial output, reported by the user. | Complete; see the [test record](../tests/results/2026-11-20-board-check.md). |
| 1 — Sensor and ESP32 | Review pins and voltage divider; display distances in the serial monitor. | Functional distance response compared with a ruler. Preserve numeric series and error cases when available. | Complete based on the user's report; quantitative characterization remains to be recorded. |
| 2 — Volume | Select a container, calibrate empty/full distances, validate, filter, and convert to liters. | Comparison with known quantities, with geometry and error documented. | In progress: identify the container and collect calibration references. See the [calibration guide](calibration.md). |
| 3 — Connection | Connect to Wi-Fi and recover from disconnections. | Test with network loss and recovery. | Pending. |
| 4 — Cloud | Select storage, authorize uploads, and preserve history. | Actual records and handling of communication errors. | Pending. |
| 5 — Dashboard | Display indicators, history, latest measurement, and alerts. | Interface using actual data, including handling of stale or missing data. | Pending. |
| 6 — Testing | Test the complete system, fix issues, and prepare the presentation. | Reproducible procedure and results with known limitations. | Pending. |

## Stage 2 calculation topics

- Review the echo-time-to-distance conversion already used in Stage 1, including units and the round trip.
- Calibrate the empty and full distances; validate calibration and clamp the percentage to 0–100%.
- Distinguish height percentage from volume percentage when required by the geometry.
- Validate readings before filtering; compare mean and median using observed data.
- Test the calculation with known volumes and record its errors.

## Suggested commits

Each commit should represent a small change that the student has reviewed. Suggested messages and exact terminal commands will be provided when a change is ready. For this initial documentation:

```text
docs: initialize project structure and English documentation
```

See [the setup guide](setup.md#3-review-the-repository-files) for the first commit's review, staging, commit, and push commands.

## Improvements after completing the original scope

We will first evaluate which actual problem warrants an improvement. Possible extensions include estimating consumption, predicting when water will run out, and detecting unusual behavior. The choice will depend on available data and remaining time; AI is not being added yet.
