CFS RFID XIAO C6 - Web UI v0.2.1

Changes from v0.2.0:
- Added UTF-8 charset to repair garbled special characters
- Replaced decorative broken glyph and removed misleading Device interface badge
- Creality logo loads from Wikimedia Commons (online); offline fallback wordmark is displayed if load fails
- Centered main / template / edit buttons via action-row wrappers
- Corrected material database list: full-width table in independent horizontal-scroll container; balanced columns and compact actions
- Kept read/write and printer push firmware and all JS business handlers unchanged

Install: flash sketch with XIAO_ESP32C6, 4MB flash, No OTA 2MB APP/2MB SPIFFS partition; do not erase flash or upload LittleFS.

IMPORTANT: The actual logo image is sourced remotely. For offline AP mode it falls back to a Creality wordmark. Supply an official logo file to embed it permanently without network access.

Backup your prior sketch; verify RFID reading, selection, template creation, live sync/push and dialog layout before final deployment.
