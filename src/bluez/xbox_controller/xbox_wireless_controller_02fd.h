#ifndef SRC_BLUEZ_XBOX_CONTROLLER_MSFT_XBOX_02FD_H
#define SRC_BLUEZ_XBOX_CONTROLLER_MSFT_XBOX_02FD_H

#include <cstdint>

/*
  the struct belows were generated using `hidrdd-code` via the following
  command:

  rexx rd.rex -d --hex 05 01 09 05 a1 01 85 01 09 01 a1 00 09 30 09 31 15 00 27
  ff ff 00 00 95 02 75 10 81 02 c0 09 01 a1 00 09 32 09 35 15 00 27 ff ff 00 00
  95 02 75 10 81 02 c0 05 02 09 c5 15 00 26 ff 03 95 01 75 0a 81 02 15 00 25 00
  75 06 95 01 81 03 05 02 09 c4 15 00 26 ff 03 95 01 75 0a 81 02 15 00 25 00 75
  06 95 01 81 03 05 01 09 39 15 01 25 08 35 00 46 3b 01 66 14 00 75 04 95 01 81
  42 75 04 95 01 15 00 25 00 35 00 45 00 65 00 81 03 05 09 19 01 29 0f 15 00 25
  01 75 01 95 0f 81 02 15 00 25 00 75 01 95 01 81 03 05 0c 0a 24 02 15 00 25 01
  95 01 75 01 81 02 15 00 25 00 75 07 95 01 81 03 05 0c 09 01 85 02 a1 01 05 0c
  0a 23 02 15 00 25 01 95 01 75 01 81 02 15 00 25 00 75 07 95 01 81 03 c0 05 0f
  09 21 85 03 a1 02 09 97 15 00 25 01 75 04 95 01 91 02 15 00 25 00 75 04 95 01
  91 03 09 70 15 00 25 64 75 08 95 04 91 02 09 50 66 01 10 55 0e 15 00 26 ff 00
  75 08 95 01 91 02 09 a7 15 00 26 ff 00 75 08 95 01 91 02 65 00 55 00 09 7c 15
  00 26 ff 00 75 08 95 01 91 02 c0 85 04 05 06 09 20 15 00 26 ff 00 75 08 95 01
  81 02 c0
 */

//--------------------------------------------------------------------------------
// Decoded Application Collection
//--------------------------------------------------------------------------------

// All structure fields should be byte-aligned...
#pragma pack(push, 1)

//--------------------------------------------------------------------------------
// Generic Desktop Page inputReport 01 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;                // Report ID = 0x01 (1)
                                   // Collection: CA:Gamepad CP:Pointer
  uint16_t GD_GamepadPointerX;     // Usage 0x00010030: X, Value = 0 to 65535
  uint16_t GD_GamepadPointerY;     // Usage 0x00010031: Y, Value = 0 to 65535
  uint16_t GD_GamepadPointerZ;     // Usage 0x00010032: Z, Value = 0 to 65535
  uint16_t GD_GamepadPointerRz;    // Usage 0x00010035: Rz, Value = 0 to 65535
                                   // Collection: CA:Gamepad
  uint16_t SIM_GamepadBrake : 10;  // Usage 0x000200C5: Brake, Value = 0 to 1023
  uint8_t : 6;                     // Pad
  uint16_t SIM_GamepadAccelerator : 10;  // Usage 0x000200C4: Accelerator, Value
                                         // = 0 to 1023
  uint8_t : 6;                           // Pad
  uint8_t
      GD_GamepadHatSwitch : 4;  // Usage 0x00010039: Hat Switch, Value = 1 to 8,
                                // Physical = (Value - 1) x 45 in degrees
  uint8_t : 4;                  // Pad
  uint8_t BTN_GamepadButton1 : 1;  // Usage 0x00090001: Button 1
                                   // Primary/trigger, Value = 0 to 1
  uint8_t BTN_GamepadButton2 : 1;  // Usage 0x00090002: Button 2 Secondary,
                                   // Value = 0 to 1
  uint8_t BTN_GamepadButton3 : 1;  // Usage 0x00090003: Button 3 Tertiary, Value
                                   // = 0 to 1
  uint8_t BTN_GamepadButton4 : 1;  // Usage 0x00090004: Button 4, Value = 0 to 1
  uint8_t BTN_GamepadButton5 : 1;  // Usage 0x00090005: Button 5, Value = 0 to 1
  uint8_t BTN_GamepadButton6 : 1;  // Usage 0x00090006: Button 6, Value = 0 to 1
  uint8_t BTN_GamepadButton7 : 1;  // Usage 0x00090007: Button 7, Value = 0 to 1
  uint8_t BTN_GamepadButton8 : 1;  // Usage 0x00090008: Button 8, Value = 0 to 1
  uint8_t BTN_GamepadButton9 : 1;  // Usage 0x00090009: Button 9, Value = 0 to 1
  uint8_t
      BTN_GamepadButton10 : 1;  // Usage 0x0009000A: Button 10, Value = 0 to 1
  uint8_t
      BTN_GamepadButton11 : 1;  // Usage 0x0009000B: Button 11, Value = 0 to 1
  uint8_t
      BTN_GamepadButton12 : 1;  // Usage 0x0009000C: Button 12, Value = 0 to 1
  uint8_t
      BTN_GamepadButton13 : 1;  // Usage 0x0009000D: Button 13, Value = 0 to 1
  uint8_t
      BTN_GamepadButton14 : 1;  // Usage 0x0009000E: Button 14, Value = 0 to 1
  uint8_t
      BTN_GamepadButton15 : 1;   // Usage 0x0009000F: Button 15, Value = 0 to 1
  uint8_t : 1;                   // Pad
  uint8_t CD_GamepadAcBack : 1;  // Usage 0x000C0224: AC Back, Value = 0 to 1
  uint8_t : 7;                   // Pad
} inputReport01_t;

//--------------------------------------------------------------------------------
// Consumer Page inputReport 02 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;  // Report ID = 0x02 (2)
                     // Collection: CA:Gamepad CA:ConsumerControl
  uint8_t CD_GamepadConsumerControlAcHome : 1;  // Usage 0x000C0223: AC Home,
                                                // Value = 0 to 1
  uint8_t : 7;                                  // Pad
} inputReport02_t;

//--------------------------------------------------------------------------------
// Decoded Application Collection
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Generic Device Controls Page inputReport 04 (Device --> Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;                    // Report ID = 0x04 (4)
                                       // Collection: CA:Gamepad
  uint8_t GDC_GamepadBatteryStrength;  // Usage 0x00060020: Battery Strength,
                                       // Value = 0 to 255
} inputReport04_t;

//--------------------------------------------------------------------------------
// Physical Input Device Page outputReport 03 (Device <-- Host)
//--------------------------------------------------------------------------------

typedef struct {
  uint8_t reportId;  // Report ID = 0x03 (3)
                     // Collection: CA:Gamepad CL:SetEffectReport
  uint8_t
      PID_GamepadSetEffectReportDcEnableActuators : 4;  // Usage 0x000F0097: DC
                                                        // Enable Actuators,
                                                        // Value = 0 to 1
  uint8_t : 4;                                          // Pad
  uint8_t
      PID_GamepadSetEffectReportMagnitude[4];  // Usage 0x000F0070: Magnitude,
                                               // Value = 0 to 100
  uint8_t PID_GamepadSetEffectReportDuration;  // Usage 0x000F0050: Duration,
                                               // Value = 0 to 255, Physical =
                                               // Value in 10⁻² s units
  uint8_t
      PID_GamepadSetEffectReportStartDelay;  // Usage 0x000F00A7: Start Delay,
                                             // Value = 0 to 255, Physical =
                                             // Value in 10⁻² s units
  uint8_t PID_GamepadSetEffectReportLoopCount;  // Usage 0x000F007C: Loop Count,
                                                // Value = 0 to 255
} outputReport03_t;

#pragma pack(pop)

#endif  // SRC_BLUEZ_XBOX_CONTROLLER_MSFT_XBOX_02FD_H
