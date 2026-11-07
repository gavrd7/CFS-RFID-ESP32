# Generic ESP32-C6 boards — source-only community builds

The **Seeed Studio XIAO ESP32-C6 is the only officially hardware-tested board**. This folder describes an experimental path for other ESP32-C6 development boards. There are **no precompiled generic firmware binaries** and no guarantee that a particular development board will work without adaptations.

## Before compiling

1. Download the **Generic Source** ZIP from the release. Extract the **entire** `CFS_RFID_XIAO_C6_TFT` sketch folder, keeping all nested headers and `partitions.csv` beside the sketch.
2. Open `CFS_RFID_XIAO_C6_TFT.ino` in Arduino IDE and edit **`board_config.h`** in the same sketch directory.
3. The generic source ZIP already selects `CFS_BOARD_GENERIC_ESP32C6 = 1` and disables `CFS_BOARD_XIAO_ESP32C6`. Set **all nine** GPIO placeholders (initially `-1`) for your actual board and wiring. The compiler deliberately rejects incomplete mappings.
4. Match RC522 to RFID SPI SCK/MISO/MOSI, SS and RST; match the ST7789 TFT to the shared SPI bus, its separate CS and DC/RST; map the optional speaker output to an available GPIO. Check your board's pinout for strapping, boot mode, USB, integrated flash, RF/antenna and other reserved functions. **Do not assume the D-pin labels from the XIAO have the same GPIO numbers on your board.**
5. Select the **correct ESP32-C6 board** and **actual flash capacity** in Arduino IDE; do not blindly select XIAO. Confirm compatibility of the included custom OTA partition table (`app0` and `app1`, 0x1B0000 bytes each, plus 0x90000 bytes LittleFS) with your board's flash and boot configuration **before flashing**. The current partition table is for **4 MB flash**. Devices with smaller or differently arranged flash require a separately verified partition layout.
6. Build and flash **over USB** first. Verify boot, Wi-Fi, display, RFID read and printer communication before considering OTA. Back up data before any reflash or partition change; `LittleFS.begin(true)` can format an unmountable filesystem.

## Safety and support

- **Never upload `CFS-RFID-XIAO-ESP32C6-OTA.bin` to a non-XIAO board.** The public precompiled image has XIAO wiring and antenna controls and is tested only for that model.
- Generic boards intentionally **do not run the XIAO-specific GPIO3/GPIO14 antenna switch initialization**. If your development board needs special RF-switch setup, adapt that board's own hardware initialization separately.
- The generic-source ZIP is **source code only**. It has not been compiled or tested against every ESP32-C6 board, and it does not constitute a safe OTA image.
- Printer SSH/SCP and database synchronization logic are shared unchanged with the XIAO build. **Do not test printer database writes while checking a new board**; start with reads and keep printer/database backups.
- The web/OTA endpoints have no authenticated HTTPS or signed firmware verification. Operate only on a trusted LAN.
- When reporting compatibility, include the precise board model/revision, flash size, ESP32 core version, GPIO mapping, power/voltage details, and which functions passed. **Do not include passwords**.

For installation and migration details see [INSTALL.md](INSTALL.md). For known security limitations see [SECURITY.md](SECURITY.md).
