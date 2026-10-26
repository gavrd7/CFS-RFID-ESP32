First merged proof build

1. Keep the existing working push-agent firmware available so you can revert.
2. Rewire RC522 and buzzer according to WIRING.txt.
3. Install/retain LibSSH-ESP32 in Arduino IDE. The RFID AES/MFRC522/UI sources
   are bundled under src/ exactly as in the existing RFID project, with the UI
   modified to use this same XIAO for printer sync/push.
4. Select Seeed Studio XIAO ESP32C6 and a partition layout with enough LittleFS
   space for material_database.json. The merged binary size must be checked at
   compile time before choosing an OTA-capable partition layout.
5. Flash over USB for this first test. Open Serial Monitor at 115200.
6. Test in this order: web UI -> RC522 tag read -> tag write -> /test SSH ->
   Sync Live From Printer -> Push to Printer without reboot -> optional reboot.

Networking: AP is explicitly enabled/disabled by the existing AP toggle.
When disabled, this build does not automatically start a fallback AP if STA
connection fails.


v0.1.5 changes from the known-good v0.1 base:
- XIAO ESP32-C6 onboard ceramic Wi-Fi antenna selected (external U.FL disabled).
- MFRC522 receiver gain left at the library/default setting, matching the known-good v0.1 behaviour.
- No other RFID read/write/parser behaviour changed from v0.1.
