# CFS RFID — XIAO ESP32-C6, RC522, ST7789 and Creality printer push

**Firmware baseline:** v0.3.9 (locally tested by the project owner). This archive is a publication-preparation starter, NOT an out-of-the-box validated public release.

Consolidates the original RFID writer and printer push agent into one Seeed XIAO ESP32-C6, with a 1.54-inch ST7789 240×240 screen.

## Features

- CFS-compatible RC522 read/write and passive tag inspection
- Explicit web-controlled Read and Write actions, with a read-only idle mode
- Web UI for filament management and creation from templates
- Material label cache for the TFT, including custom entries when indexed
- 3D printer database pull/push initiated explicitly through the web UI
- TFT feedback for tag operations and printer transfers
- Explicit access-point enable/disable setting

## Project layout

- `firmware/CFS_RFID_XIAO_C6_TFT/` — full Arduino sketch, embedded web UI, bundled AES, MFRC522, material sources and TFT artwork
- `docs/Wiring_Reference.pdf` and `.docx` — wiring reference (the drawing describes the v0.3.7 pinout, which is unchanged in v0.3.9)
- `docs/history/` — previous build notes retained for traceability
- `docs/INSTALL.md` — libraries, board settings and flash procedure
- `docs/SECURITY.md` — publication, credentials and printer safety
- `docs/UPSTREAM_AND_LICENSE.md` — origin, license and attribution checks still required

## Safety-critical functionality

Printer database operations are **manual**: pull/sync and push occur only at the operator's explicit request in the browser. Do not enable auto-sync, or alter the established printer transfer/validation routines as a casual refactor. Back up the printer database before testing a push.

## Release status and licensing

The included application and embedded libraries are **not automatically relicensable** merely because they have been packaged together. Check original repository and upstream component licenses, and add accurate copyright notices, attribution and an appropriate project license before publishing. Creality branding is a third-party trademark; confirm its use is appropriate for your public fork.

See `docs/INSTALL.md` and `docs/SECURITY.md` before attempting a flash.
