# Stage 0 — Board-check result

- **Date:** November 20, 2026.
- **Test operator and evidence source:** student; result reported in the project chat. The assistant did not perform the physical test or independently capture the serial monitor.
- **Computer:** Windows, following the decision to move hardware development from macOS.
- **Board:** photographed classic ESP32 development board with USB-C; exact manufacturer remains unconfirmed.
- **Sketch:** [board_check.ino](../../firmware/board_check/board_check.ino).
- **Code reference:** local commit `94ea9b3` (`feat: add ESP32 serial board check`) contains the prepared sketch. The user reported completing the commit.

## Procedure and expected behavior

Run the board-check sketch on the ESP32 and view its output in Serial Monitor. The sketch initializes serial output at 115200 baud and prints uptime in whole seconds with a two-second delay between iterations.

## Reported observation

The user reported resolving the Windows connection issue, running the program, and receiving:

```text
Uptime (s): 1
Uptime (s): 3
Uptime (s): 5
```

**Result:** pass based on the user's report. The increasing values match the expected progression. Actual message arrival intervals were not independently timed.

The previous Windows diagnosis found a CP2102 with missing-driver code 28; COM7 and COM8 were Bluetooth ports. The user reported resolution, but the final assigned COM number, driver version, and full Windows ESP32 core version were not supplied.

## Scope and next step

This result supports a working firmware upload and serial-output path on the student's computer. It does not validate sensor wiring, distance measurements, Wi-Fi, cloud storage, or the dashboard. Next, confirm the sensor circuit materials and power connection before wiring the HC-SR04.
