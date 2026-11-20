# ESP32 firmware

This directory contains sketches that run on the microcontroller.

## Stage 0: board check

Open [board_check/board_check.ino](board_check/board_check.ino) in Arduino IDE. Keep the sensor disconnected, select **ESP32 Dev Module** and the confirmed USB serial port, then verify and upload the sketch. Open Serial Monitor at **115200 baud**.

`setup()` runs once after startup or reset. It initializes serial output and prints a startup message after a short delay. `loop()` then repeatedly prints elapsed time in seconds, with a two-second pause. `millis()` returns elapsed milliseconds; dividing by 1000 converts them to whole seconds. `delay()` pauses this simple diagnostic sketch; scheduling will be revisited when the application needs to handle other tasks.

Expected output includes increasing `Uptime (s):` values approximately every two seconds. The initial number depends on when Serial Monitor is opened. Press EN after opening the monitor to see startup output again if needed.

**Test status:** sketch prepared, but compilation, upload, and serial output have not been confirmed. The assistant has not performed a physical upload. Report the observed output before marking this check as passed. See the [official serial API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/serial.html).

## Stage 1: sensor measurements

In Stage 1, we will create the `water_tank_monitor/water_tank_monitor.ino` sketch. Arduino requires the sketch directory and its main file to have the same name.

The first sensor program will only measure distances and display them over the serial port. Calculations, Wi-Fi, and cloud uploads will be added in their respective stages, with an explanation of each change.

**Application status:** sensor code and GPIO selection are pending. The photograph indicates a classic ESP32 development board with USB-C. The user has confirmed selecting **ESP32 Dev Module**. See the [setup evidence](../docs/setup.md).
