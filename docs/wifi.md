# Stage 3 — Windows hotspot connection

**Status: accepted based on the student's report.** The ESP32 connected and recovered after hotspot shutdown/restart without interrupting measurements. The assistant did not perform the physical test. Calibration and the 2 L usable maximum are unchanged; the next [cloud stage](cloud.md) now has prepared upload code with live storage verification pending.

## Configure the Windows hotspot

1. Open **Settings → Network & internet → Mobile hotspot**.
2. Select the computer's working internet connection under **Share my internet connection from** and choose **Wi-Fi** under **Share over**.
3. Under **Properties → Edit**, set the hotspot name and password and select the **2.4 GHz** network band for this classic ESP32.
4. Save and enable the hotspot. Keep the computer awake while testing.

UI labels can vary with Windows version. If no compatible band can be selected, investigate the adapter's hotspot options before changing ESP32 drivers. [Microsoft's hotspot guide](https://support.microsoft.com/en-gb/windows/experience/connectivity-networking/use-your-windows-device-as-a-mobile-hotspot).

## Local credentials

In `firmware/water_tank_monitor/`, copy `secrets.example.h` to **secrets.h** if the local file does not exist. A blank local copy was prepared for this workspace without overwriting an existing file. Edit only `secrets.h`:

```cpp
const char WIFI_SSID[] = "YOUR_HOTSPOT_NAME";
const char WIFI_PASSWORD[] = "YOUR_HOTSPOT_PASSWORD";
```

Keep the surrounding quotes. Match the name and password exactly, including case. The existing `.gitignore` excludes `secrets.h`, while the example remains blank and versioned. Do not paste passwords into the project chat. Firmware logs do not print credentials.

The sketch uses the local file when available. Otherwise it falls back to the blank example, reports that Wi-Fi is not configured, and continues measuring. No additional library installation is required: `WiFi.h` comes with the ESP32 core.

## How the code works

- `WiFi.mode(WIFI_STA)` makes the ESP32 join the computer's hotspot as a client.
- `WiFi.begin()` starts a connection attempt. There is no loop that waits indefinitely for Wi-Fi before allowing measurements.
- `maintainWiFi()` checks connection status on each main-loop iteration and prints the IP when connected.
- If disconnected, it starts another attempt every 15 seconds using `millis()`. Built-in auto-reconnect is disabled so only this retry mechanism controls attempts.
- Sensor sampling and filtering continue when Wi-Fi is unavailable. The existing sensor delay and ECHO timeout still apply; this is not a hard real-time scheduler.

API reference: [Espressif Wi-Fi documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html).

## Manual verification

1. Fill in the local credentials, reload the sketch, compile and upload. Open Serial Monitor at **115200 baud**.
2. With the hotspot enabled, look for `Wi-Fi connected | IP:` followed by the address actually assigned to this ESP32. No specific IP value is assumed.
3. Confirm water measurements still appear and remain consistent with the unchanged calibration.
4. Disable the hotspot until the monitor reports disconnection. Confirm measurement output continues.
5. Re-enable the hotspot, allow time for retry and network negotiation, and confirm a new connected message without resetting the ESP32.
6. Report the actual output. Joining a hotspot and obtaining a local IP alone do not prove working public internet access; external service communication is checked in Stage 4.

The student subsequently reported successful connection and recovery while readings continued. No exact timing or output series was supplied. The assistant did not configure the hotspot, access the real credentials, or connect the board.
