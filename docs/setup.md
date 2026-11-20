# Stage 0 — Environment setup

**Status: complete based on the user's successful Windows board-check report.** The earlier environment reviews and troubleshooting entries below record the setup history, rather than the current driver state. See the [test record](../tests/results/2026-11-20-board-check.md).

## Local environment review — November 18, 2026

| Item | Evidence found | Remaining work |
| --- | --- | --- |
| Git | `git version 2.55.0.windows.5` | Ready for initial work. |
| Repository | Branch `main`, no commits; `origin` points to the GitHub project. | First commit and push by the student. Remote access has not been tested. |
| VS Code | The `code` command is available. | Open the project directory to edit it there if desired. |
| Arduino IDE | Executable and metadata for version 2.3.10. | Open it and verify that it works. |
| ESP32 core | Version 3.3.12 directory under `Arduino15/packages/esp32/hardware/esp32`. | Confirm in Boards Manager; no compilation has been performed. |
| Node.js / npm | Versions 25.2.1 / 11.6.2. | Not needed at this stage. |
| Board and serial port | Not confirmed. | Identify the model and check the port in Windows. |

Finding installed files does not prove that the board communicates correctly.

## Board identification and USB check — November 20, 2026

- The user confirmed that `/dev/cu.usbserial-0001` disappears when the board is disconnected and reappears when it is reconnected. This identifies the board's USB serial port for the current session.
- The Arduino IDE screenshot uses macOS-style port names. This hardware check is being performed on a different environment from the Windows workspace reviewed above; its IDE and ESP32 core versions still need to be confirmed.
- The board photograph shows a module labeled `ESP-32`, a USB-C connector, and BOOT and EN buttons. Its markings and layout indicate a classic ESP32 development board; the exact manufacturer and flash capacity are not established by the photograph.
- Recommended initial board profile: **ESP32 Dev Module**. Selection, compilation, and upload have not yet been confirmed. Espressif documents generic development profiles in its [Tools Menu guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/guides/tools_menu.html).
- Follow-up: the user confirmed **esp32 by Espressif Systems** is installed, reported its version as `4.0`, and confirmed selecting **ESP32 Dev Module**. The complete release string and the test computer's IDE version have not been independently verified. The Windows core version above does not describe this installation.
- Completed follow-up: the user later reported running the [board-check sketch](../firmware/board_check/board_check.ino) on Windows and receiving the expected uptime output. The full Windows core version and assigned COM number were not supplied.

The initial USB detection report alone did not establish firmware upload or serial output. The later board-check report provides user-reported evidence for those steps. Sensor measurement remains untested. No physical tests were performed by the assistant.

### Upload and serial-check procedure

1. Open `firmware/board_check/board_check.ino` on the computer connected to the ESP32. If transferring the sketch manually, preserve the `board_check` directory and `board_check.ino` file names.
2. Confirm **ESP32 Dev Module** and the port on the computer currently connected to the board. `/dev/cu.usbserial-0001` was confirmed on macOS; the Windows COM number must be read from the current port list. The initial Windows driver issue described below has been reported resolved.
3. Click **Verify** (checkmark). A successful compilation checks that the sketch builds; it does not upload anything.
4. Click **Upload** (right arrow). This writes the sketch to the board, replacing its previous application. Wait for completion and check for errors.
5. Open **Tools → Serial Monitor** and select **115200 baud**, matching `Serial.begin(115200)`.
6. Look for increasing `Uptime (s):` messages approximately every two seconds. These are expected results, not observations already obtained. If the startup line was missed, press **EN** once with the monitor open.
7. Report the output. If compilation or upload fails, preserve the error text for diagnosis before changing settings or proceeding to the sensor.

### Switch to Windows: missing CP2102 driver

The user chose the Windows computer for further development. Before the successful board check, read-only Windows device queries found:

- **COM7** and **COM8** are Bluetooth serial ports, not the ESP32's USB serial port.
- **CP2102 USB to UART Bridge Controller**, hardware ID `VID_10C4&PID_EA60`, is present with **problem code 28**, and no driver version or provider is recorded.
- Microsoft documents [code 28](https://learn.microsoft.com/en-us/windows-hardware/drivers/install/cm-prob-failed-install) as missing device drivers. Firmware upload was blocked at that point.

The recommended resolution was to download **CP210x Universal Windows Driver** from [Silicon Labs](https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads), extract the archive, and install it through Windows Device Manager: right-click the CP2102 entry, select **Update driver → Browse my computer for drivers**, select the extracted directory, include subfolders, and follow the installation prompts.

After installation, the expected check is a **Silicon Labs CP210x USB to UART Bridge (COM...)** entry that disappears and reappears when the board is disconnected and reconnected. The user subsequently reported resolving the issue and running the sketch successfully. The assigned COM number and installed driver version were not supplied. No driver was installed by the assistant.

The earlier macOS core version did not establish the Windows core version. During the first sensor sketch's local build check, Arduino CLI selected **esp32:esp32 4.0.0** from the installed Windows packages, establishing the current local core version. The assigned Windows COM number remains to be recorded. Compilation checks do not establish physical upload or sensor behavior.

## 1. Confirm Arduino IDE and the core

The IDE is the application where we write, compile, and upload firmware. The core provides ESP32-specific tools and functions; an Arduino Uno board is not required to use Arduino IDE.

1. Open **Arduino IDE** from the Windows Start menu or the macOS Applications folder, depending on the computer used for the hardware test.
2. Go to **Help → About** and check the version.
3. Open **Boards Manager** from the sidebar.
4. Search for `esp32` and check that **esp32 by Espressif Systems** is installed. Record its version.
5. Keep the installed version for this project; an update is unnecessary if it works with our board.

If the package does not appear, go to **File → Preferences** and add the following address to **Additional Boards Manager URLs**, keeping any existing entries:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Then return to Boards Manager and install **esp32 by Espressif Systems** from the stable index. This follows [Espressif's official instructions](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html). If the IDE does not open, consult [Arduino's official guide](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE) before reinstalling.

## 2. Identify the board and check USB

1. Copy the text on the metal module and the labels on both sides of the circuit board. Identify the USB connector.
2. With the sensor still disconnected, connect only the ESP32 using a USB **data** cable.
3. On Windows, open **Device Manager** and observe which device appears when the board is connected. It usually appears under **Ports (COM & LPT)**; some variants use native USB and may appear differently. On macOS, compare the Arduino IDE port list before and after connecting the board.
4. Record the port. `COM5` is a Windows example; `/dev/cu.usbserial-0001` is the port confirmed by the user during the current macOS test. Port names may change on another computer.
5. In Arduino IDE, open **Tools → Port** and check whether the port appears.

Select a profile for the identified chip family: ESP32, ESP32-C3, and ESP32-S3 require different configurations. For the photographed board, the recommended starting profile is **ESP32 Dev Module**, pending compilation and upload verification.

If only an LED lights up and no device appears, try another data cable and USB port. If Windows shows an unknown device, first identify its chip or hardware IDs to select the appropriate official driver. Avoid installing unrelated drivers.

## 3. Review the repository files

From PowerShell, inside the project directory:

```powershell
git status --short --branch
```

Before the first commit, expect `No commits yet on main` and `??` entries for new files. `??` means that Git does not track those files yet.

To open the directory in VS Code:

```powershell
code .
```

There is no need to run `npm install`. After reviewing the documentation, the student can run the following commands to create and push the first commit:

```powershell
git diff
git add "README.md" ".gitignore" "firmware/" "dashboard/" "docs/" "tests/"
git diff --cached
git commit -m "docs: initialize project structure and English documentation"
git push -u origin main
```

`git diff` shows unstaged changes to tracked files. Because these files are initially untracked, it may show nothing. After `git add`, use `git diff --cached` to review the exact contents of the upcoming commit. Run the commit and push commands only after reviewing that output. If the diff viewer opens a pager, press `q` to exit it.

## Stage completion criteria

- [x] Review the existing files and available tools.
- [x] Prepare the structure, README, and initial documentation.
- [x] Confirm that Arduino IDE opens and shows the ESP32 core as installed, based on the user's report.
- [x] Identify the board family from the photograph and verify USB detection through the user's disconnect/reconnect check; exact manufacturer remains unconfirmed.
- [x] Select **ESP32 Dev Module**, based on the user's report.
- [x] Resolve Windows USB communication, based on the user's report after troubleshooting the CP2102 driver.
- [x] Establish a working Windows compilation/upload path, based on the user's successful sketch run. Exact core version and COM number remain to be recorded.
- [x] Run the board-check sketch and confirm repeated serial output, based on the user's reported `1`, `3`, and `5` second uptime values.
- [x] Create the initial documentation commit; it is present in the local repository history.

Stage 1 starts with confirming the circuit materials and power connection before wiring the sensor.
