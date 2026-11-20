# Wiring — Stage 1 signal connections

**Status: signal assembly reported by the user; inspection and sensor power verification pending.** The photographed board is a classic ESP32 development board with labels D18 and D19. Its USB board check passed according to the user's report. The user reports completing the signal connections and having a multimeter. No assembly photograph has been reviewed, resistor values have not been independently checked, and no sensor circuit test has been reported.

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

## Verify the board supply with a multimeter

**Status: user reported 5.17 V after following the board-supply measurement procedure.** The exact pin label was not included in the report and still needs to be recorded. This checks only the USB-powered board, not the sensor circuit under load. The assistant did not independently measure the voltage.

1. With USB disconnected, take a clear photograph of the current assembly for review. Temporarily disconnect the ESP32's jumper connections to the sensor and divider, leaving that circuit unpowered. Keep sensor VCC disconnected.
2. Connect the black meter lead to **COM** and the red lead to the voltage-measurement socket, usually marked **V/Ω**. Do not use the 10 A socket.
3. Select **DC voltage**, marked **V⎓**. For a manual-ranging meter, use the **20 V** range; for an autoranging meter, use DC voltage mode. Do not use current, resistance, or continuity mode for this powered measurement.
4. Identify **GND** and the pin labeled **VIN** or **5V** on the board itself. Do not substitute 3V3 or guess by pin position. If the labels cannot be read, obtain a clear photograph before continuing.
5. Power only the ESP32 through USB. Put the black probe on GND and the red probe on the identified VIN/5V pin. Avoid bridging adjacent pins with a probe tip.
6. Record the displayed voltage and the exact pin label. A value near 5 V is expected for a USB-derived supply, possibly reduced by protection components, but the actual value must be measured on this board.
7. Disconnect USB again before reconnecting any jumper wires. Share the reading and assembly photograph before powering the sensor circuit.

| Check | Result |
| --- | --- |
| Signal assembly | Reported complete by user; visual review pending. |
| Multimeter availability | Confirmed by user. |
| Board supply-to-GND voltage | 5.17 V, reported by user; exact measured pin label to confirm. |
| Sensor supply under load | Not tested. |
| Divider output with constant test input | User reported 3.2 V; calculated nominal value is 3.102 V for a 5.17 V input. Exact resistor values, tolerances, and meter accuracy have not been checked independently. |
| Divider output during ECHO pulses | Not tested. |

## Test the divider with a constant voltage

Before connecting the divider to GPIO19, test it separately using the same board pin where the user measured 5.17 V. A constant input allows the divider ratio to be checked with a DC multimeter; this is not a sensor ECHO measurement.

1. Disconnect USB. Keep the sensor disconnected, including VCC and ECHO, and disconnect GPIO19 from the divider junction. Do not connect any signal GPIO during this test.
2. Connect the lower end of R2 (3.3 kΩ) to ESP32 GND.
3. Connect a temporary jumper from the measured 5.17 V board supply pin to the outer end of R1 (2.2 kΩ), where ECHO would normally connect. Keep the R1/R2 junction separate from this supply connection.
4. Confirm the junction has no connection to GPIO19 or the sensor. Connect USB.
5. With the meter in DC voltage mode, put the black probe on ESP32 GND and the red probe on the R1/R2 junction.
6. Record the reading. The calculated value is `5.17 × 3.3 / (2.2 + 3.3) = 3.102 V`, approximately **3.10 V**. This is expected, not an observed result; resistor tolerances and measurement accuracy affect the actual value.
7. Disconnect USB, then remove the temporary supply-to-R1 jumper. Do not restore sensor ECHO or GPIO19 until that jumper is removed and the result has been reviewed.

```text
Measured supply ----[ R1: 2.2 kΩ ]----+---- Red voltage probe
                                     |
                                [ R2: 3.3 kΩ ]
                                     |
ESP32 GND ---------------------------+---- Black voltage probe

Sensor and GPIO19: DISCONNECTED during this test
```

The user should report the junction voltage, the original supply pin label, and a clear assembly photograph. If the reading differs substantially from the calculated value, investigate resistor values and breadboard connections before continuing.

**Reported result:** the user measured **3.2 V** at the divider junction. This demonstrates attenuation in the reported constant-input test; it is not a measurement of the sensor's ECHO pulse peak. The difference from the nominal 3.102 V has not been attributed to a verified cause. Resistor tolerance, actual input voltage during this test, meter accuracy, and display rounding may contribute.

In the final signal circuit, the junction connects to **D19 / GPIO19**. ECHO pulses drive the divider input, and GPIO19 receives their reduced voltage levels. The program will measure pulse duration to calculate distance; voltage amplitude itself does not encode distance. Remove the temporary supply-to-R1 jumper with USB disconnected before restoring ECHO or GPIO19.

## Next steps after inspection and voltage verification

Review the assembly and the board's measured supply voltage first. Then document the accepted power connection, reconnect the signal circuit with USB disconnected, and explain sensor VCC wiring and the sensor sketch before testing against a flat surface. The board-check sketch does not trigger the sensor or measure distance. An ordinary DC-voltage reading of ECHO is not a reliable way to check its pulse peak voltage.

Do not assume that every pin labeled VIN provides 5 V from USB. Its use depends on the board. The [ESP32-DevKitC guide](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html) describes a specific board; it does not identify the one we own.

The diagram above specifies the chosen signal GPIOs. Add a photograph after the physical assembly has been inspected, and record the verified power connection and actual test results when available.
