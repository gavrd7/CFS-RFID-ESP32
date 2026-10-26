CFS RFID XIAO C6 - Web UI v0.2.2

Changes from v0.2.1:
- Centers the Settings Save button underneath the configuration panel.
- Changes ambiguous "Submit Query" button text to "Save Settings".
- Uses type=button to avoid default submit behaviour, retaining the same saveConf() handler.

No changes to the RFID, LittleFS, printer push, Wi-Fi/AP logic, or JavaScript handlers.

Install: XIAO_ESP32C6, 4MB flash, No OTA 2MB APP/2MB SPIFFS, USB CDC enabled. Do not erase flash or upload LittleFS.

Please validate saving settings in the web UI before deploying.
