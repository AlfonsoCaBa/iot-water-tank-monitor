# Wiring — Stage 1 signal connections

**Status: signal connections specified; physical assembly and sensor power verification pending.** The photographed board is a classic ESP32 development board with labels D18 and D19. Its USB board check passed according to the user's report. The user reports having resistors; verify the 2.2 kΩ and 3.3 kΩ values before assembly. No sensor circuit test has been reported.

## Sensor pin functions

| Pin | Function | Planned connection |
| --- | --- | --- |
| VCC | HC-SR04 power supply. | Leave disconnected during this assembly step; the 5 V supply pin must be verified first. |
| GND | Common electrical reference. | ESP32 ground and voltage divider ground. |
| TRIG | Input that requests a measurement. | ESP32 **D18 / GPIO18**, configured as an output in the future sensor sketch. |
| ECHO | Output pulse representing the echo time. | Voltage divider junction, then ESP32 **D19 / GPIO19**, configured as an input in the future sensor sketch. |

Follow the labels printed on the module; an image's orientation is not a substitute for identifying them.

D18 and D19 refer to the GPIO labels, not the eighteenth and nineteenth physical header positions. These classic ESP32 GPIOs are exposed on the photographed board; selecting them does not imply the sensor has been tested. Common ground means that the sensor, voltage divider, and ESP32 share the same electrical reference.

## Why two resistors are needed

The standard HC-SR04 uses a 5 V supply; we will treat ECHO as a signal that may reach that voltage. ESP32 GPIO pins use 3.3 V logic, so **ECHO will not be connected directly to them**. See the [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) and the [classic ESP32 electrical specifications](https://documentation.espressif.com/esp32_datasheet_en.html). We will check the documentation for the actual board model.

A voltage divider uses two resistors to produce a lower voltage at their junction:

```text
HC-SR04 ECHO ----[ R1: 2.2 kΩ ]----+---- ESP32 D19 / GPIO19
                                  |
                             [ R2: 3.3 kΩ ]
                                  |
HC-SR04 GND -----------------------+---- ESP32 GND

HC-SR04 TRIG --------------------------- ESP32 D18 / GPIO18
HC-SR04 VCC ---------------------------- NOT CONNECTED YET
```

The input connects to the junction between the resistors. R2 connects that junction to ground; a single resistor in series is not sufficient.

```text
V_input = V_ECHO × R2 / (R1 + R2)
        = 5 × 3.3 / (2.2 + 3.3)
        = 3.0 V nominal
```

This is a design calculation, not a multimeter measurement. Common ground allows both modules to interpret signals against the same reference. TRIG will be tested with a 3.3 V output, and we will verify that the module responds.

## Assemble the unpowered signal circuit

1. Disconnect USB and any other power source. Identify HC-SR04 VCC, TRIG, ECHO, and GND from its printed labels.
2. Identify a 2.2 kΩ resistor (R1) and a 3.3 kΩ resistor (R2). They have no polarity, but their positions in the divider must not be swapped.
3. Connect sensor GND to ESP32 GND. If using a breadboard rail as common ground, connect that rail to ESP32 GND; its colored stripe does not create an electrical connection by itself.
4. Connect TRIG to D18.
5. Connect ECHO to one end of R1. Connect R1's other end to a free breadboard node; this will be the divider junction.
6. Connect one end of R2 to that same junction, and its other end to common ground.
7. Connect D19 to the junction. ECHO must reach D19 through R1, not by a direct jumper that bypasses it.
8. Leave VCC disconnected and keep USB disconnected. Obtain an assembly photograph and confirm access to a multimeter before the power-verification step.

On a typical breadboard, a numbered row contains two separate groups of five connected holes, split by the center gap. Put the three junction connections in one connected group. Place each resistor's two ends in different electrical groups, or the resistor will be bypassed. Power rails may also be split; verify continuity where necessary.

## Next steps after inspection

Confirm the board's sensor power connection and ground using its labeling and appropriate voltage checks. Only then connect sensor VCC, reconnect USB, and test the sensor sketch against a flat surface. The board-check sketch does not trigger the sensor or measure distance.

Do not assume that every pin labeled VIN provides 5 V from USB. Its use depends on the board. The [ESP32-DevKitC guide](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html) describes a specific board; it does not identify the one we own.

The diagram above specifies the chosen signal GPIOs. Add a photograph after the physical assembly has been inspected, and record the verified power connection and actual test results when available.
