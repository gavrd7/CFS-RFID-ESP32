CFS RFID XIAO C6 - Web UI Preview v0.2.0
======================================
Based directly on v0.1.5 (known working RF logic, default RC522 gain, onboard Wi-Fi antenna).

This first-stage build changes ONLY src/WWW/html.h:
  - responsive dark theme, rounded panels, inputs and dialogs
  - new title / explanatory text
  - original DOM IDs, script handlers, material templates, RFID operations,
    printer Sync Live, Push to Printer, configuration and update dialogs retained

It does NOT yet implement TFT driver or display UI. Do not wire a TFT yet.
It has NOT been built/flashed on actual hardware in this environment.

TEST BEFORE DEPLOYMENT:
1) Arduino IDE verify (XIAO ESP32C6, original working partition settings).
2) Open page on desktop AND phone; open Settings, Material Database, Templates.
3) Choose a new filament; verify Apply and Read Tag flows with test tags only.
4) Verify database editor; Sync Live; Push to Printer after backup.
5) Confirm explicit AP enable/disable still behaves identically.
6) Confirm heap usage and RC522 read reliability unchanged.

ROLLBACK: Flash your original v0.1.5 archive (not included/modified).
CHANGE SCOPE: src/WWW/html.h only. No firmware code or protocol change.
