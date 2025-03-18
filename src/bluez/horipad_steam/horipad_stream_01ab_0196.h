#ifndef SRC_BLUEZ_HORIPAD_STEAM_HORIPAD_STEAM_01AB_H
#define SRC_BLUEZ_HORIPAD_STEAM_HORIPAD_STEAM_01AB_H

#include <cstdint>

/*
  the struct belows were generated using `hidrdd-code` via the following
  command:

  rexx rd.rex -d --hex 05 01 09 05 a1 01 85 07 a1 00 09 30 09 31 09 32 09 35 15
  00 26 ff 00 75 08 95 04 81 02 c0 09 39 15 00 25 07 35 00 46 3b 01 65 14 75 04
  95 01 81 42 05 09 19 01 29 14 15 00 25 01 75 01 95 14 81 02 05 02 15 00 26 ff
  00 09 c4 09 c5 95 02 75 08 81 02 06 00 ff 09 20 95 26 81 02 85 05 09 21 95 20
  91 02 85 12 09 22 95 3f 81 02 09 23 91 02 85 14 09 26 95 3f 81 02 09 27 91 02
  85 10 09 24 95 3f 81 02 85 0f 09 28 95 3f 91 02 c0
 */

//--------------------------------------------------------------------------------
// Decoded Application Collection
//--------------------------------------------------------------------------------

// All structure fields should be byte-aligned...
#pragma pack(push, 1)

//--------------------------------------------------------------------------------
// Generic Desktop Page inputReport 07 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;                 // Report ID = 0x07 (7)
                                    // Collection: CA:Gamepad CP:
  uint8_t GD_GamepadX;              // Usage 0x00010030: X, Value = 0 to 255
  uint8_t GD_GamepadY;              // Usage 0x00010031: Y, Value = 0 to 255
  uint8_t GD_GamepadZ;              // Usage 0x00010032: Z, Value = 0 to 255
  uint8_t GD_GamepadRz;             // Usage 0x00010035: Rz, Value = 0 to 255
                                    // Collection: CA:Gamepad
  uint8_t GD_GamepadHatSwitch : 4;  // Usage 0x00010039: Hat Switch, Value = 0
                                    // to 7, Physical = Value x 45 in degrees
  uint8_t BTN_GamepadButton1 : 1;   // Usage 0x00090001: Button 1
                                    // Primary/trigger, Value = 0 to 1, Physical
                                    // = Value x 315 in degrees
  uint8_t
      BTN_GamepadButton2 : 1;  // Usage 0x00090002: Button 2 Secondary, Value =
                               // 0 to 1, Physical = Value x 315 in degrees
  uint8_t
      BTN_GamepadButton3 : 1;  // Usage 0x00090003: Button 3 Tertiary, Value = 0
                               // to 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton4 : 1;   // Usage 0x00090004: Button 4, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton5 : 1;   // Usage 0x00090005: Button 5, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton6 : 1;   // Usage 0x00090006: Button 6, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton7 : 1;   // Usage 0x00090007: Button 7, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton8 : 1;   // Usage 0x00090008: Button 8, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton9 : 1;   // Usage 0x00090009: Button 9, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton10 : 1;  // Usage 0x0009000A: Button 10, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton11 : 1;  // Usage 0x0009000B: Button 11, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton12 : 1;  // Usage 0x0009000C: Button 12, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton13 : 1;  // Usage 0x0009000D: Button 13, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton14 : 1;  // Usage 0x0009000E: Button 14, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton15 : 1;  // Usage 0x0009000F: Button 15, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton16 : 1;  // Usage 0x00090010: Button 16, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton17 : 1;  // Usage 0x00090011: Button 17, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton18 : 1;  // Usage 0x00090012: Button 18, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton19 : 1;  // Usage 0x00090013: Button 19, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t BTN_GamepadButton20 : 1;  // Usage 0x00090014: Button 20, Value = 0 to
                                    // 1, Physical = Value x 315 in degrees
  uint8_t
      SIM_GamepadAccelerator;   // Usage 0x000200C4: Accelerator, Value = 0 to
                                // 255, Physical = Value x 21 / 17 in degrees
  uint8_t SIM_GamepadBrake;     // Usage 0x000200C5: Brake, Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
  uint8_t VEN_Gamepad0020[38];  // Usage 0xFF000020: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} inputReport07_t;

//--------------------------------------------------------------------------------
// Vendor-defined inputReport 12 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x12 (18)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0022[63];  // Usage 0xFF000022: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} inputReport12_t;

//--------------------------------------------------------------------------------
// Vendor-defined inputReport 14 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x14 (20)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0026[63];  // Usage 0xFF000026: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} inputReport14_t;

//--------------------------------------------------------------------------------
// Vendor-defined inputReport 10 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x10 (16)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0024[63];  // Usage 0xFF000024: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} inputReport10_t;

//--------------------------------------------------------------------------------
// Vendor-defined outputReport 05 (Device <-- Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x05 (5)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0021[32];  // Usage 0xFF000021: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} outputReport05_t;

//--------------------------------------------------------------------------------
// Vendor-defined outputReport 12 (Device <-- Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x12 (18)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0023[63];  // Usage 0xFF000023: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} outputReport12_t;

//--------------------------------------------------------------------------------
// Vendor-defined outputReport 14 (Device <-- Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x14 (20)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0027[63];  // Usage 0xFF000027: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} outputReport14_t;

//--------------------------------------------------------------------------------
// Vendor-defined outputReport 0F (Device <-- Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;             // Report ID = 0x0F (15)
                                // Collection: CA:Gamepad
  uint8_t VEN_Gamepad0028[63];  // Usage 0xFF000028: , Value = 0 to 255,
                                // Physical = Value x 21 / 17 in degrees
} outputReport0F_t;

#pragma pack(pop)

#endif  // SRC_BLUEZ_HORIPAD_STEAM_HORIPAD_STEAM_01AB_H
