v0.3.2 — TFT read-success splash redraw fix

Based on v0.3.1. Fixed double green success splash after reading a tag: when the web UI resolves a scanned material ID to a brand/name, the display now records that text but does not redraw the active splash. The existing 1700ms expiry redraws the normal tag details using the resolved friendly name.

No changes to CFS tag parsing, tag read/write operations, SSH/printer sync, web UI, GPIO mappings, Wi-Fi/AP, default RC522 gain or splash length.

Arduino settings: XIAO_ESP32C6; USB CDC On Boot enabled; No OTA (2MB APP / 2MB SPIFFS); Erase All Flash disabled. No LittleFS upload.

Not compiled or device-tested here. Save/retain the confirmed working v0.3.1 as rollback.
