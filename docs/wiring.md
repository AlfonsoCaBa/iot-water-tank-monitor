# Wiring — conceptual diagram

**Status: board identification pending.** This document explains the planned connections; GPIO numbers and a power pin have not been confirmed. The circuit has not been assembled or tested.

## Sensor pin functions

| Pin | Function | Planned connection |
| --- | --- | --- |
| VCC | HC-SR04 power supply. | A 5 V supply to be confirmed for the board. |
| GND | Common electrical reference. | ESP32 ground and voltage divider ground. |
| TRIG | Input that requests a measurement. | An ESP32 output GPIO to be selected. |
| ECHO | Output pulse representing the echo time. | Voltage divider, followed by an input GPIO to be selected. |

Follow the labels printed on the module; an image's orientation is not a substitute for identifying them.

## Why two resistors are needed

The standard HC-SR04 uses a 5 V supply; we will treat ECHO as a signal that may reach that voltage. ESP32 GPIO pins use 3.3 V logic, so **ECHO will not be connected directly to them**. See the [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) and the [classic ESP32 electrical specifications](https://documentation.espressif.com/esp32_datasheet_en.html). We will check the documentation for the actual board model.

A voltage divider uses two resistors to produce a lower voltage at their junction:

```text
HC-SR04 ECHO ----[ R1: 2.2 kΩ ]----+---- ESP32 input GPIO
                                  |
                             [ R2: 3.3 kΩ ]
                                  |
HC-SR04 GND -----------------------+---- ESP32 GND
```

The input connects to the junction between the resistors. R2 connects that junction to ground; a single resistor in series is not sufficient.

```text
V_input = V_ECHO × R2 / (R1 + R2)
        = 5 × 3.3 / (2.2 + 3.3)
        = 3.0 V nominal
```

This is a design calculation, not a multimeter measurement. Common ground allows both modules to interpret signals against the same reference. TRIG will be tested with a 3.3 V output, and we will verify that the module responds.

## Planned assembly in Stage 1

1. Identify the model and consult its pinout and power supply documentation.
2. Select an output GPIO for TRIG and an input GPIO for ECHO.
3. Disconnect USB before wiring; assemble and inspect the voltage divider.
4. Connect the grounds and signals, then check the supply connections and continuity before powering up.
5. Initially power the setup from USB, keeping the electronics dry.
6. Test against a flat surface first, then above the container.

Do not assume that every pin labeled VIN provides 5 V from USB. Its use depends on the board. The [ESP32-DevKitC guide](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html) describes a specific board; it does not identify the one we own.

In Stage 1, we will add a diagram with the actual GPIO numbers and labels, along with a photograph of the inspected assembly.
