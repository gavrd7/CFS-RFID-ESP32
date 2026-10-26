v0.3.4 TEST — display-only improvements atop user-tested v0.3.3.

- Passive reads display friendly brand and material names for IDs in the firmware's existing embedded default catalogue (40 entries), without requiring a web browser.
- Other IDs (including local custom additions not in that embedded catalogue) still show Material ID and the decoded ID. No new database reading/writing routines were introduced.
- Distinct icons for READ (tag/contactless) and WRITE (tag/pencil).
- All RFID action/state and CFS encoding logic unchanged.
- Printer sync/push code and database code/files left completely untouched.
- Do not erase LittleFS and do not upload a filesystem image.
- This build has NOT been compiled or device-tested. Test a known passive scan, explicit read, armed write with a disposable tag, and verify the protected printer features remain available.
