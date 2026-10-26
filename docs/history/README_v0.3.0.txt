# XIAO ESP32-C6 CFS RFID + Printer Push + ST7789 — integration candidate v0.3.0

Based on the working v0.2.2 source and the successful ORIGINAL-library read-only ST7789 hardware test.

Includes: original web UI, material templates/database editor, explicit AP on/off, CFS read/write, raw database streaming endpoints (/getdb, /pushdb) and SSH task. Adds display drawing and optional browser-to-screen friendly labels.

WIRING (unchanged RC522): SCK D8/GPIO19, MISO D9/GPIO20, MOSI D10/GPIO18, CS D3/GPIO21, RST D2/GPIO2. TFT: SCL D8/GPIO19, SDA D10/GPIO18, CS D4/GPIO22, DC D5/GPIO23, RST D6/GPIO16, 3V3/GND. BL connect only according to your tested module's backlight electrical requirements. Buzzer D1/GPIO1.

Libraries: Adafruit GFX Library; Adafruit ST7735 and ST7789 Library (including their dependencies); original bundled src/MFRC522 (no separate MFRC522 library). Keep the working libssh_esp32 environment.

Arduino: XIAO_ESP32C6, USB CDC enabled, 4MB flash, No OTA (2MB APP/2MB SPIFFS), erase all flash DISABLED. No separate LittleFS upload. If compile exceeds partition or flash availability, DO NOT change partition/erase without backing up settings. 

IMPORTANT: v0.3.0 is NOT independently compile-tested or hardware-validated here. Back up v0.2.2 and LittleFS data. For the initial integration check, verify boot RC522 version/antenna, TFT state, Wi-Fi and web settings; test a sacrificial tag for read then write; finally test printer sync/push using a safe database backup. Original CFS writing is unchanged and remains automatically armed when not in Read mode: keep valuable tags away during testing.

Display idle brand/type appear after choosing filament and pressing Apply on web UI. The successful read displays raw material ID by default; browser resolves friendly name after it polls /readresult. If no browser is open, raw ID is shown rather than an invented name. Screen initial layout can be improved in later revisions.

Known caveat: display drawing is kept out of SSH worker and RFID transactions; actual concurrent system behavior must be validated on hardware. Existing write routine's optimistic Write complete log is unchanged from v0.2.2; it does not yet read back to verify. Keep this in mind during tests.
