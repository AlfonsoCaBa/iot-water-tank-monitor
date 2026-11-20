# ESP32 firmware

This directory contains sketches that run on the microcontroller.

## Stage 0: board check

Open [board_check/board_check.ino](board_check/board_check.ino) in Arduino IDE. Keep the sensor disconnected, select **ESP32 Dev Module** and the confirmed USB serial port, then verify and upload the sketch. Open Serial Monitor at **115200 baud**.

`setup()` runs once after startup or reset. It initializes serial output and prints a startup message after a short delay. `loop()` then repeatedly prints elapsed time in seconds, with a two-second pause. `millis()` returns elapsed milliseconds; dividing by 1000 converts them to whole seconds. `delay()` pauses this simple diagnostic sketch; scheduling will be revisited when the application needs to handle other tasks.

Expected output includes increasing `Uptime (s):` values approximately every two seconds. The initial number depends on when Serial Monitor is opened. Press EN after opening the monitor to see startup output again if needed.

**Test status:** passed based on the user's report of a successful Windows sketch run with uptime values `1`, `3`, and `5`. See the [test record](../tests/results/2026-11-20-board-check.md). The assistant did not perform the upload or independently observe the monitor. See the [official serial API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/serial.html).

## Stage 1: sensor measurements

Open [water_tank_monitor/water_tank_monitor.ino](water_tank_monitor/water_tank_monitor.ino). Arduino requires the sketch directory and its main file to have the same name.

This first sensor program only measures distance and displays it over the serial port. Volume calculations, filtering, Wi-Fi, and cloud uploads will be added in their respective stages, with an explanation of each change.

### How the measurement works

- `TRIG_PIN = 18` is an output; `ECHO_PIN = 19` is an input reached through the voltage divider.
- The sketch starts TRIG low, holds it high for 10 microseconds, and returns it low to request a measurement.
- `pulseIn(ECHO_PIN, HIGH, 30000)` measures the high pulse in microseconds. A zero result is treated as a missing or incomplete echo, not an empty tank.
- Sound travels approximately 0.0343 cm per microsecond near room temperature. `distance = echo time × 0.0343 / 2` accounts for the round trip. This approximation is temperature dependent and is not a calibration result.
- Computed distances outside the specified 2–400 cm range are rejected. This basic range check does not detect every incorrect reflection within that range.
- `delay(500)` spaces readings by at least half a second; the echo measurement adds a small amount of time. No smoothing is applied yet, so fluctuations remain visible.

### First physical test

1. Follow the [final sensor connection instructions](../docs/wiring.md#final-sensor-connection-and-first-distance-test), removing the temporary divider test jumper before connecting the sensor.
2. Select **ESP32 Dev Module** and the current Windows COM port. Verify and upload this sketch, then open Serial Monitor at **115200 baud**.
3. Aim the sensor perpendicular to a broad, flat target, such as a wall or large rigid board. Start at about 20 cm, measured from the front of the transducers. Keep the target away from the sensor's minimum distance.
4. Record 5–10 consecutive readings without moving the target. Repeat at approximately 10 cm and 30 cm, measuring the actual reference distances with a ruler.
5. Report the actual readings and any errors. Do not infer that the water-level measurement works from this dry target test alone.

Sources: [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf), [Arduino pulseIn reference](https://docs.arduino.cc/language-reference/en/functions/advanced-io/pulseIn/).

**Local compilation:** Arduino CLI successfully compiled this sketch for `esp32:esp32:esp32` using the installed ESP32 core **4.0.0**. Reported usage: 292717 bytes of program storage and 23208 bytes of global-variable memory. Build artifacts and the local CLI configuration are stored under the ignored `firmware/build/` directory; no upload was performed by the assistant.

**Physical test status:** functional distance test passed based on the user's report: the sketch ran, responded to different target distances, and was checked with a ruler. Exact reference distances, reading series, and errors were not supplied, so no numeric accuracy or stability claim is made. See the [sensor test record](../tests/results/2026-11-20-sensor-distance.md). The assistant did not perform the physical test.

## Stage 2: calibration pending

The sketch remains a distance-only program. Before modifying it to calculate level and liters, collect the container geometry, usable capacity, and empty/full reference distances described in the [calibration guide](../docs/calibration.md).
