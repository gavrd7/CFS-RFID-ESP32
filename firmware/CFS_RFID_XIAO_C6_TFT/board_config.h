#pragma once

// =========================================================================
// 1. ARCHITECTURE DETECTION & PROFILE SELECTION
// =========================================================================

#if defined(CONFIG_IDF_TARGET_ESP32C6)
  // For the ESP32-C6, select exactly one profile:
  // Set to 1 for the official Seeed Studio board, 0 for generic C6 boards.
  #define CFS_BOARD_XIAO_ESP32C6      1

#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  // For the ESP32-S3, select exactly one profile:
  // Set to 1 for Super Mini / Waveshare Zero, 0 for generic S3 boards.
  #define CFS_BOARD_S3_SUPERMINI      0
  #define CFS_BOARD_S3_ZERO           1

#else
  #error "Unsupported target architecture. This firmware supports ESP32-C6 and ESP32-S3."
#endif


// =========================================================================
// 2. ESP32-C6 HARDWARE PROFILES
// =========================================================================
#if defined(CONFIG_IDF_TARGET_ESP32C6)

  #if CFS_BOARD_XIAO_ESP32C6
    // Officially tested reference wiring (XIAO C6 GPIO numbers)
    #define BOARD_NAME                  "XIAO ESP32-C6"
    #define RFID_SCK_PIN                19
    #define RFID_MISO_PIN               20
    #define RFID_MOSI_PIN               18
    #define RFID_SS_PIN                 21
    #define RFID_RST_PIN                 2
    #define SPK_PIN                      1
    #define TFT_CS_PIN                  22
    #define TFT_DC_PIN                  23
    #define TFT_RST_PIN                 16
    #define CFS_HAS_XIAO_ANTENNA_SWITCH 1

  #else
    // Generic / Community ESP32-C6 defaults
    #define BOARD_NAME                  "Generic ESP32-C6"
    #define RFID_SCK_PIN                19
    #define RFID_MISO_PIN               20
    #define RFID_MOSI_PIN               18
    #define RFID_SS_PIN                 21
    #define RFID_RST_PIN                 2
    #define SPK_PIN                      1
    #define TFT_CS_PIN                  22
    #define TFT_DC_PIN                  23
    #define TFT_RST_PIN                 16
    #define CFS_HAS_XIAO_ANTENNA_SWITCH 0
  #endif

// =========================================================================
// 3. ESP32-S3 HARDWARE PROFILES
// =========================================================================
#elif defined(CONFIG_IDF_TARGET_ESP32S3)

  #if CFS_BOARD_S3_SUPERMINI
    // Custom mapping for ESP32-S3 Super Mini
    #define BOARD_NAME                  "ESP32-S3 Super Mini"

  #elif CFS_BOARD_S3_ZERO
    // Custom mapping for Waveshare ESP32-S3 Zero
    #define BOARD_NAME                  "Waveshare ESP32-S3 Zero"

  #else
    // Placeholder for future generic or full-sized ESP32-S3 Dev Kits
    #define BOARD_NAME                  "Generic ESP32-S3"

  #endif
    #define RFID_SCK_PIN                12
    #define RFID_MISO_PIN               13
    #define RFID_MOSI_PIN               11
    #define RFID_SS_PIN                 10
    #define RFID_RST_PIN                 6
    #define SPK_PIN                      1
    #define TFT_CS_PIN                   2
    #define TFT_DC_PIN                   4
    #define TFT_RST_PIN                  5
    #define CFS_HAS_XIAO_ANTENNA_SWITCH 0

#endif


// =========================================================================
// 4. GLOBAL SANITY CHECK
// =========================================================================
#if RFID_SCK_PIN < 0 || RFID_MISO_PIN < 0 || RFID_MOSI_PIN < 0 || RFID_SS_PIN < 0 || RFID_RST_PIN < 0 || SPK_PIN < 0 || TFT_CS_PIN < 0 || TFT_DC_PIN < 0 || TFT_RST_PIN < 0
  #error "Complete all required GPIO pin assignments in board_config.h."
#endif