v0.3.3 TEST CANDIDATE – RFID UI & modes ONLY

Base: user-tested v0.3.2. No modifications to printer SSH/SCP code, database code, material template functions, CFS cipher or tag encoding/decoding routines. The /spooldata handler has only RFID-state/UI lines changed; its database persistence logic is unchanged. The existing browser explicit Read Tag intentionally still fills the form fields; idle passive scans do NOT.

Modes:
- Idle: passive read-only scan, preview on TFT, never writes and does not change browser read result or selection.
- Read Tag: explicitly armed, populates existing web read results.
- Apply: stores the selected spool data; now disarms RFID write.
- Write Tag: explicitly arms one operation (requires Apply since boot).
- Cancel: returns to passive mode.
- After a read/write: returns to passive mode.
- UID latch: waits for tag removal before another operation; test multiple tags and repeated presents.

Safety: The original CFS encoding and database transfer code are unchanged. Before trusting write results, test disposable tags and confirm that a failed authentication never triggers a write. Acknowledged MIFARE writes still do not have read-back verification.

NOT hardware compiled/tested. USB CDC On Boot enabled, 4 MB flash, No OTA 2MB APP/2MB SPIFFS, Erase All Flash disabled. Libraries as v0.3.2.
