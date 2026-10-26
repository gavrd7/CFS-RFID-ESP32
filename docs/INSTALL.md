# Build and flash

1. Install Arduino IDE and the ESP32 boards package; the working setup used **XIAO_ESP32C6** with ESP32 core 3.3.x.
2. Install **Adafruit GFX Library**, **Adafruit ST7735 and ST7789 Library**, and compatible **libssh_esp32** and its dependencies. The firmware bundles its own MFRC522 implementation in `src/` and AES-related sources.
3. Open `firmware/CFS_RFID_XIAO_C6_TFT/CFS_RFID_XIAO_C6_TFT.ino`.
4. Select XIAO_ESP32C6, USB CDC On Boot **Enabled**, Flash Size **4 MB**, Partition Scheme **No OTA (2MB APP / 2MB SPIFFS)**, and Erase All Flash **Disabled** when upgrading an already configured device.
5. Confirm wiring using `Wiring_Reference.pdf`. Verify the ST7789 BL/backlight current limiting for your own specific module before powering it.
6. Review credentials, hostname, printer model/host, and AP setting near the top of the sketch. Public starter values are placeholders and are not intended as operational credentials.
7. Compile and upload using USB. Open Serial Monitor at **115200 baud**.
8. No separate LittleFS upload is normally required; the firmware initializes local storage. The sketch calls `LittleFS.begin(true)`, so mounting failure may cause a format. Back up configuration first.
9. First test passive reads and explicit web reads using known tags. Use a disposable tag for write tests. Back up and validate printer data before explicitly syncing/pushing.

**Important:** The No OTA partition plan means USB is the reliable firmware installation method. Check the full available flash/partition arrangement if you change the board settings.

For repository publication, supply independently verifiable versions/sources for `libssh_esp32` and other third-party dependencies; they are **not** bundled in this archive.
