v0.3.5 TEST — DISPLAY-ONLY CUSTOM FILAMENT LABEL CACHE

Baseline: v0.3.4 TEST (user confirmed all functional tests passed).

Changes: New isolated /display/cache endpoint and /display_labels.txt file, with a small add-on browser script which mirrors material ID/brand/name from its ALREADY LOADED local material list. Refresh when the browser is open and its material list changes (15 second check). Cached labels survive restart. Fallback to built-in labels and then material ID.

No printer sync, printer push, SSH/SCP, database save, parsing, or original material manager functions changed. Existing src files are byte-identical except an appended standalone display-only script in src/WWW/html.h. No automatic printer contact.

CAUTION: this is an uncompiled test candidate. A browser must be connected at least once to prime/update the cache; after adding a material through the web UI, keep the page open for approximately 15 seconds to allow display metadata refresh. The cache is presentation-only.

Test: 1) browse web UI and check Serial Monitor for `Display label cache updated`; 2) passive-scan a newly created/custom CFS tag and check friendly name; 3) reboot with browser closed and scan again; 4) verify read/write and printer features remain unchanged.

Preserve: XIAO_ESP32C6, No OTA 2MB APP/2MB SPIFFS, Erase All Flash Disabled. No filesystem upload. Keep v0.3.4 backup.
