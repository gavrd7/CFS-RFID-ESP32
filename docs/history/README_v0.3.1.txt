v0.3.1: TFT result overlay added to the working v0.3.0 build.

A 1700 ms green check for successful read/write; red exclamation for read/write or RFID errors.
Overlay uses millis(), does not block HTTP/SSH/event handling.
After the overlay the normal filament/tag information view returns.
Writing now checks each MFRC522 write return code (including sector trailer read/write) before declaring success. This is acknowledgement checking, NOT read-back verification. Existing tag encoding, database and printer-push implementation retained.

Hardware and libraries: same as v0.3.0.
Arduino: XIAO_ESP32C6, USB CDC On Boot Enabled, No OTA (2MB APP/2MB SPIFFS), Erase All Flash Disabled.
No LittleFS upload needed.

Caution: No Arduino compiler/physical hardware verification in this environment. Test read first, then write only to a disposable tag.
