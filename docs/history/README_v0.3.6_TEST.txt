CFS RFID XIAO ESP32-C6 TFT v0.3.6A TEST

Based on tested v0.3.5. Changes restricted to TFT presentation,
Creality mark bitmap, and a separate browser-to-TFT transfer notification.

Printer SCP/SSH functions and data handling are unchanged. Original
syncLiveFromPrinter(), pushToPrinter(), fetchRetry(), mmMsg() and
material database functions are unmodified.

Printer-transfer notice observes existing browser messages: it requires
the web page to remain open. It does not stop RFID scans, block SPI, or
prevent users from tapping; the 'Do not scan tags' warning is advisory.
The TFT has no real percentage-complete telemetry and intentionally
shows no fabricated progress.

Validate: idle/scan/read/write screens, custom label lookup, transfer
screens while syncing and pushing, and all previous functions.

This has not been compiled or hardware tested. Keep v0.3.5 available.
