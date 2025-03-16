#ifndef SRC_BLUEZ_PS5_DUAL_SENSE_DUAL_SENSE_0CE6_H_
#define SRC_BLUEZ_PS5_DUAL_SENSE_DUAL_SENSE_0CE6_H_

#include <cstdint>

/*
  the struct belows were generated using `hidrdd-code` via the following
  command:

  rexx rd.rex -d --hex 05 01 09 05 a1 01 85 01 09 30 09 31 09 32 09 35 09 33 09
  34 15 00 26 ff 00 75 08 95 06 81 02 06 00 ff 09 20 95 01 81 02 05 01 09 39 15
  00 25 07 35 00 46 3b 01 65 14 75 04 95 01 81 42 65 00 05 09 19 01 29 0f 15 00
  25 01 75 01 95 0f 81 02 06 00 ff 09 21 95 0d 81 02 06 00 ff 09 22 15 00 26 ff
  00 75 08 95 34 81 02 85 02 09 23 95 2f 91 02 85 05 09 33 95 28 b1 02 85 08 09
  34 95 2f b1 02 85 09 09 24 95 13 b1 02 85 0a 09 25 95 1a b1 02 85 20 09 26 95
  3f b1 02 85 21 09 27 95 04 b1 02 85 22 09 40 95 3f b1 02 85 80 09 28 95 3f b1
  02 85 81 09 29 95 3f b1 02 85 82 09 2a 95 09 b1 02 85 83 09 2b 95 3f b1 02 85
  84 09 2c 95 3f b1 02 85 85 09 2d 95 02 b1 02 85 a0 09 2e 95 01 b1 02 85 e0 09
  2f 95 3f b1 02 85 f0 09 30 95 3f b1 02 85 f1 09 31 95 3f b1 02 85 f2 09 32 95
  0f b1 02 85 f4 09 35 95 3f b1 02 85 f5 09 36 95 03 b1 02 c0

 */

//--------------------------------------------------------------------------------
// Decoded Application Collection
//--------------------------------------------------------------------------------

// All structure fields should be byte-aligned...
#pragma pack(push, 1)

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 05 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x05 (5)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0033[40];  // Usage 0xFF000033: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport05_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 08 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x08 (8)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0034[47];  // Usage 0xFF000034: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport08_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 09 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x09 (9)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0024[19];  // Usage 0xFF000024: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport09_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 0A (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x0A (10)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0025[26];  // Usage 0xFF000025: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport0A_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 20 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x20 (32)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0026[63];  // Usage 0xFF000026: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport20_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 21 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;            // Report ID = 0x21 (33)
                               // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0027[4];  // Usage 0xFF000027: , Value = 0 to 255, Physical
                               // = Value x 21 / 17
} featureReport21_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 22 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x22 (34)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0040[63];  // Usage 0xFF000040: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport22_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 80 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x80 (128)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0028[63];  // Usage 0xFF000028: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport80_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 81 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x81 (129)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0029[63];  // Usage 0xFF000029: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport81_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 82 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;            // Report ID = 0x82 (130)
                               // Collection: CA:Gamepad
  uint8_t VEN_Gamepad002A[9];  // Usage 0xFF00002A: , Value = 0 to 255, Physical
                               // = Value x 21 / 17
} featureReport82_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 83 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x83 (131)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad002B[63];  // Usage 0xFF00002B: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport83_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 84 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x84 (132)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad002C[63];  // Usage 0xFF00002C: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReport84_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport 85 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;            // Report ID = 0x85 (133)
                               // Collection: CA:Gamepad
  uint8_t VEN_Gamepad002D[2];  // Usage 0xFF00002D: , Value = 0 to 255, Physical
                               // = Value x 21 / 17
} featureReport85_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport A0 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;         // Report ID = 0xA0 (160)
                            // Collection: CA:Gamepad
  uint8_t VEN_Gamepad002E;  // Usage 0xFF00002E: , Value = 0 to 255, Physical =
                            // Value x 21 / 17
} featureReportA0_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport E0 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0xE0 (224)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad002F[63];  // Usage 0xFF00002F: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReportE0_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport F0 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0xF0 (240)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0030[63];  // Usage 0xFF000030: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReportF0_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport F1 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0xF1 (241)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0031[63];  // Usage 0xFF000031: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReportF1_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport F2 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0xF2 (242)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0032[15];  // Usage 0xFF000032: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReportF2_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport F4 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0xF4 (244)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0035[63];  // Usage 0xFF000035: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} featureReportF4_t;

//--------------------------------------------------------------------------------
// Vendor-defined featureReport F5 (Device <-> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;            // Report ID = 0xF5 (245)
                               // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0036[3];  // Usage 0xFF000036: , Value = 0 to 255, Physical
                               // = Value x 21 / 17
} featureReportF5_t;

//--------------------------------------------------------------------------------
// Generic Desktop Page inputReport 01 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;                 // Report ID = 0x01 (1)
                                    // Collection: CA:Gamepad
  uint8_t GD_GamepadX;              // Usage 0x00010030: X, Value = 0 to 255
  uint8_t GD_GamepadY;              // Usage 0x00010031: Y, Value = 0 to 255
  uint8_t GD_GamepadZ;              // Usage 0x00010032: Z, Value = 0 to 255
  uint8_t GD_GamepadRz;             // Usage 0x00010035: Rz, Value = 0 to 255
  uint8_t GD_GamepadRx;             // Usage 0x00010033: Rx, Value = 0 to 255
  uint8_t GD_GamepadRy;             // Usage 0x00010034: Ry, Value = 0 to 255
  uint8_t VEN_Gamepad0020;          // Usage 0xFF000020: , Value = 0 to 255
  uint8_t GD_GamepadHatSwitch : 4;  // Usage 0x00010039: Hat Switch, Value = 0
                                    // to 7, Physical = Value x 45 in degrees
  uint8_t
      BTN_GamepadButton1 : 1;  // Usage 0x00090001: Button 1 Primary/trigger,
                               // Value = 0 to 1, Physical = Value x 315
  uint8_t BTN_GamepadButton2 : 1;  // Usage 0x00090002: Button 2 Secondary,
                                   // Value = 0 to 1, Physical = Value x 315
  uint8_t BTN_GamepadButton3 : 1;  // Usage 0x00090003: Button 3 Tertiary, Value
                                   // = 0 to 1, Physical = Value x 315
  uint8_t BTN_GamepadButton4 : 1;  // Usage 0x00090004: Button 4, Value = 0 to
                                   // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton5 : 1;  // Usage 0x00090005: Button 5, Value = 0 to
                                   // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton6 : 1;  // Usage 0x00090006: Button 6, Value = 0 to
                                   // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton7 : 1;  // Usage 0x00090007: Button 7, Value = 0 to
                                   // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton8 : 1;  // Usage 0x00090008: Button 8, Value = 0 to
                                   // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton9 : 1;  // Usage 0x00090009: Button 9, Value = 0 to
                                   // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton10 : 1;  // Usage 0x0009000A: Button 10, Value = 0 to
                                    // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton11 : 1;  // Usage 0x0009000B: Button 11, Value = 0 to
                                    // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton12 : 1;  // Usage 0x0009000C: Button 12, Value = 0 to
                                    // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton13 : 1;  // Usage 0x0009000D: Button 13, Value = 0 to
                                    // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton14 : 1;  // Usage 0x0009000E: Button 14, Value = 0 to
                                    // 1, Physical = Value x 315
  uint8_t BTN_GamepadButton15 : 1;  // Usage 0x0009000F: Button 15, Value = 0 to
                                    // 1, Physical = Value x 315
  uint8_t VEN_Gamepad0021 : 1;   // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00211 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00212 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00213 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00214 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00215 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00216 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00217 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00218 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad00219 : 1;  // Usage 0xFF000021: , Value = 0 to 1, Physical
                                 // = Value x 315
  uint8_t VEN_Gamepad002110 : 1;  // Usage 0xFF000021: , Value = 0 to 1,
                                  // Physical = Value x 315
  uint8_t VEN_Gamepad002111 : 1;  // Usage 0xFF000021: , Value = 0 to 1,
                                  // Physical = Value x 315
  uint8_t VEN_Gamepad002112 : 1;  // Usage 0xFF000021: , Value = 0 to 1,
                                  // Physical = Value x 315
  uint8_t VEN_Gamepad0022[52];    // Usage 0xFF000022: , Value = 0 to 255,
                                  // Physical = Value x 21 / 17
} inputReport01_t;

//--------------------------------------------------------------------------------
// Vendor-defined outputReport 02 (Device <-- Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x02 (2)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0023[47];  // Usage 0xFF000023: , Value = 0 to 255,
                                // Physical = Value x 21 / 17
} outputReport02_t;

#pragma pack(pop)

#endif  // SRC_BLUEZ_PS5_DUAL_SENSE_DUAL_SENSE_0CE6_H_