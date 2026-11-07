#pragma once

// CFS-RFID hardware selection. The Seeed XIAO ESP32-C6 remains the only
// hardware-tested target. For other ESP32-C6 development boards, edit the two
// switches below and provide GPIO numbers in the generic section.
//
// Use exactly one profile. Do NOT use a XIAO OTA .bin on a different board.
#define CFS_BOARD_XIAO_ESP32C6     1
#define CFS_BOARD_GENERIC_ESP32C6  0

#if (CFS_BOARD_XIAO_ESP32C6 + CFS_BOARD_GENERIC_ESP32C6) != 1
  #error "Select exactly one ESP32-C6 hardware profile."
#endif

#if CFS_BOARD_XIAO_ESP32C6
  // Officially tested reference wiring. GPIO numbers, not D-pin labels.
  #define RFID_SCK_PIN   19
  #define RFID_MISO_PIN  20
  #define RFID_MOSI_PIN  18
  #define RFID_SS_PIN    21
  #define RFID_RST_PIN    2
  #define SPK_PIN         1
  #define TFT_CS_PIN     22
  #define TFT_DC_PIN     23
  #define TFT_RST_PIN    16
  #define CFS_HAS_XIAO_ANTENNA_SWITCH 1

#elif CFS_BOARD_GENERIC_ESP32C6
  // COMMUNITY / UNTESTED DEFAULTS: Start from the working XIAO GPIO numbers.
  // ESP32-C6 GPIO functions are largely portable via the GPIO matrix, but
  // pin exposure and onboard connections differ between development boards.
  // REVIEW ALL NINE VALUES against your exact board schematic and wiring.
  // GPIO16 is often UART0 TX; avoid conflicts with serial peripherals.
  // RFID and TFT share SCK/MOSI but use separate CS pins.
  #define RFID_SCK_PIN   19
  #define RFID_MISO_PIN  20
  #define RFID_MOSI_PIN  18
  #define RFID_SS_PIN    21
  #define RFID_RST_PIN    2
  #define SPK_PIN         1
  #define TFT_CS_PIN     22
  #define TFT_DC_PIN     23
  #define TFT_RST_PIN    16
  #define CFS_HAS_XIAO_ANTENNA_SWITCH 0

  // Reject invalid unset pins if a user manually removes an assignment.
  #if RFID_SCK_PIN < 0 || RFID_MISO_PIN < 0 || RFID_MOSI_PIN < 0 || RFID_SS_PIN < 0 || RFID_RST_PIN < 0 || SPK_PIN < 0 || TFT_CS_PIN < 0 || TFT_DC_PIN < 0 || TFT_RST_PIN < 0
    #error "Complete the generic ESP32-C6 GPIO mappings in board_config.h."
  #endif
#endif
