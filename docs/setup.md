# Stage 0 — Environment setup

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
- Next: confirm **esp32 by Espressif Systems** in Boards Manager, record its version on the test computer, select **ESP32 Dev Module**, and select the verified serial port. Keep other settings at their defaults for now.

USB detection is user-reported evidence. It does not establish that firmware upload, serial output, or sensor measurement works. No physical tests were performed by the assistant.

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
- [ ] Confirm that Arduino IDE opens and shows the ESP32 core as installed.
- [x] Identify the board family from the photograph and verify USB detection through the user's disconnect/reconnect check; exact manufacturer remains unconfirmed.
- [ ] Review the documentation and create the first commit to save this progress if desired.

Stage 1 begins after the environment is confirmed and the materials needed to connect the sensor are available.
