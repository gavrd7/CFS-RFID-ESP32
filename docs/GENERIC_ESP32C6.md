# Generic ESP32-C6 boards — source-only community builds

The **Seeed Studio XIAO ESP32-C6 is the only officially hardware-tested board**. This folder describes an experimental path for other ESP32-C6 development boards. There are **no precompiled generic firmware binaries** and no guarantee that a particular development board will work without adaptations.

## Before flashing: board and wiring checklist

The generic package comes with **pre-populated GPIO assignments taken from the tested XIAO wiring**. These are *suggested starting values*, not a verified mapping for every ESP32-C6 board. The code can compile without changing the pin numbers, but you must still verify that those GPIOs are physically available, appropriately wired and free of board-level conflicts before flashing.

| Function | Generic default (GPIO) | Check on your board |
| --- | ---: | --- |
| RC522 SPI SCK | 19 | Exposed GPIO and suitable SPI clock routing |
| RC522 SPI MISO | 20 | Exposed GPIO and suitable SPI input routing |
| RC522 SPI MOSI | 18 | Exposed GPIO and suitable SPI output routing |
| RC522 SS/CS | 21 | Free GPIO, separate from TFT CS |
| RC522 reset | 2 | Free output-capable GPIO |
| Optional speaker signal | 1 | Free output-capable GPIO for the speaker |
| ST7789 TFT CS | 22 | Exposed free GPIO, separate from RC522 CS |
| ST7789 TFT DC | 23 | Exposed free output-capable GPIO |
| ST7789 TFT reset | 16 | Free output-capable GPIO; check UART0 TX conflicts |

The RC522 and ST7789 use the **same SPI SCK/MOSI signals** in the current design. TFT MISO is not used. Verify the voltage supply and backlight requirements of your exact modules; RC522 logic is **3.3V**. Do not assign pins used for onboard flash, USB, boot strapping, or other reserved functions. The generic profile does **not** control the XIAO RF antenna switch (GPIO3/GPIO14).

## Exact user procedure

1. Download the **Generic Source** ZIP from a release *that includes it* and extract it. The ZIP contains a sketch folder plus this guide. Do **not** use the XIAO precompiled `.bin`.
2. Open the extracted `CFS_RFID_XIAO_C6_TFT/CFS_RFID_XIAO_C6_TFT.ino` in Arduino IDE. Keep all companion headers and subfolders in place.
3. Review `board_config.h`. The generic ZIP has `CFS_BOARD_GENERIC_ESP32C6 = 1` and `CFS_BOARD_XIAO_ESP32C6 = 0`. **All nine GPIO values are pre-populated using the tested XIAO assignments**. If your other ESP32-C6 board exposes those pins and you wired the peripherals identically, you may keep them. Otherwise, edit only the affected GPIO numbers. Check that no two independent signals overlap. These defaults do **not** mean your specific development board has been tested.
4. Install the ESP32 Arduino board support and the dependencies listed in [INSTALL.md](INSTALL.md). Choose the **actual ESP32-C6 board** in Arduino IDE rather than selecting the XIAO automatically. Verify that the pin numbering matches that board definition.
5. Confirm the module has **at least 4MB of flash** and that the provided `partitions.csv` (two 0x1B0000 OTA application slots plus 0x90000 filesystem) is appropriate for its flash configuration. Check that your build actually selects the custom partition table, and fits in the selected app partition. A 4MB flash label by itself does not guarantee an arbitrary board's boot/flash configuration is compatible.
6. Check your AP and Wi-Fi settings and change default access credentials as appropriate. Printer SSH credentials are configured through the web Settings page after flashing; **do not embed real passwords in a public source archive**.
7. Choose **USB upload**, compile/verify, and flash the generic build to the actual board. If migrating from another partition layout, **back up existing LittleFS data and settings first**. The sketch uses `LittleFS.begin(true)`, which may format an unmountable filesystem.
8. Test in this order: serial boot output, network and Settings, TFT display, passive RFID reading, and **read-only** printer synchronization. Only then assess tag writing, storage and future OTA behaviour on that particular board.
9. Keep a wired recovery method available. Never upload a precompiled XIAO OTA binary to a generic board, even if its GPIO assignments look identical.

For a generic ESP32-C6 build, the firmware can potentially run without any changes to printer/database logic. Espressif's ESP32-C6 DevKitC-1 also exposes GPIO18–23, illustrating why these defaults can be useful, but it has not been tested with this firmware. **GPIO mapping alone is not a guarantee**; differences in flash, antenna circuits, boot pins and module wiring still require checking. Consult the [ESP32-C6 datasheet](https://documentation.espressif.com/esp32-c6_datasheet_en.html) and the [ESP32-C6-DevKitC-1 pinout](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32c6/esp32-c6-devkitc-1/user_guide.html).

## Safety and support

- **Never upload `CFS-RFID-XIAO-ESP32C6-OTA.bin` to a non-XIAO board.** The public precompiled image has XIAO wiring and antenna controls and is tested only for that model.
- Generic boards intentionally **do not run the XIAO-specific GPIO3/GPIO14 antenna switch initialization**. If your development board needs special RF-switch setup, adapt that board's own hardware initialization separately.
- The generic-source ZIP is **source code only**. It has not been compiled or tested against every ESP32-C6 board, and it does not constitute a safe OTA image.
- Printer SSH/SCP and database synchronization logic are shared unchanged with the XIAO build. **Do not test printer database writes while checking a new board**; start with reads and keep printer/database backups.
- The web/OTA endpoints have no authenticated HTTPS or signed firmware verification. Operate only on a trusted LAN.
- When reporting compatibility, include the precise board model/revision, flash size, ESP32 core version, GPIO mapping, power/voltage details, and which functions passed. **Do not include passwords**.

For installation and migration details see [INSTALL.md](INSTALL.md). For known security limitations see [SECURITY.md](SECURITY.md).
