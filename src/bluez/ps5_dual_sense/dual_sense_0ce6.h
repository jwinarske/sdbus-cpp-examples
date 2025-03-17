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

enum class Direction : uint8_t {
  North = 0,
  NorthEast,
  East,
  SouthEast,
  South,
  SouthWest,
  West,
  NorthWest,
  None = 8
};

enum class PowerState : uint8_t {
  Discharging = 0x00,          // Use PowerPercent
  Charging = 0x01,             // Use PowerPercent
  Complete = 0x02,             // PowerPercent not valid? assume 100%?
  AbnormalVoltage = 0x0A,      // PowerPercent not valid?
  AbnormalTemperature = 0x0B,  // PowerPercent not valid?
  ChargingError = 0x0F         // PowerPercent not valid?
};

enum class MuteLight : uint8_t {
  Off = 0,
  On,
  Breathing,
  DoNothing,  // literally nothing, this input is ignored,
              // though it might be a faster blink in other versions
  NoAction4,
  NoAction5,
  NoAction6,
  NoAction7 = 7
};

enum class LightBrightness : uint8_t {
  Bright = 0,
  Mid,
  Dim,
  NoAction3,
  NoAction4,
  NoAction5,
  NoAction6,
  NoAction7 = 7
};

enum class LightFadeAnimation : uint8_t {
  Nothing = 0,
  FadeIn,  // from black to blue
  FadeOut  // from blue to black
};

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

template <int N>
struct BTCRC {
  uint8_t Buff[N - 4];
  uint32_t CRC;
};

struct TouchFingerData {
  uint32_t Index : 7;
  uint32_t NotTouching : 1;
  uint32_t FingerX : 12;
  uint32_t FingerY : 12;
};

struct TouchData {
  TouchFingerData Finger[2];
  uint8_t Timestamp;
};

struct BTSimpleGetStateData {
  uint8_t LeftStickX;
  uint8_t LeftStickY;
  uint8_t RightStickX;
  uint8_t RightStickY;
  Direction DPad : 4;
  uint8_t ButtonSquare : 1;
  uint8_t ButtonCross : 1;
  uint8_t ButtonCircle : 1;
  uint8_t ButtonTriangle : 1;
  uint8_t ButtonL1 : 1;
  uint8_t ButtonR1 : 1;
  uint8_t ButtonL2 : 1;
  uint8_t ButtonR2 : 1;
  uint8_t ButtonShare : 1;
  uint8_t ButtonOptions : 1;
  uint8_t ButtonL3 : 1;
  uint8_t ButtonR3 : 1;
  uint8_t ButtonHome : 1;
  uint8_t ButtonPad : 1;
  uint8_t Counter : 6;
  uint8_t TriggerLeft;
  uint8_t TriggerRight;
};

struct USBGetStateData {
  uint8_t LeftStickX;
  uint8_t LeftStickY;
  uint8_t RightStickX;
  uint8_t RightStickY;
  uint8_t TriggerLeft;
  uint8_t TriggerRight;
  uint8_t SeqNo;
  Direction DPad : 4;
  uint8_t ButtonSquare : 1;
  uint8_t ButtonCross : 1;
  uint8_t ButtonCircle : 1;
  uint8_t ButtonTriangle : 1;
  uint8_t ButtonL1 : 1;
  uint8_t ButtonR1 : 1;
  uint8_t ButtonL2 : 1;
  uint8_t ButtonR2 : 1;
  uint8_t ButtonCreate : 1;
  uint8_t ButtonOptions : 1;
  uint8_t ButtonL3 : 1;
  uint8_t ButtonR3 : 1;
  uint8_t ButtonHome : 1;
  uint8_t ButtonPad : 1;
  uint8_t ButtonMute : 1;
  uint8_t UNK1 : 1;
  uint8_t ButtonLeftFunction : 1;
  uint8_t ButtonRightFunction : 1;
  uint8_t ButtonLeftPaddle : 1;
  uint8_t ButtonRightPaddle : 1;
  uint8_t UNK2;
  uint32_t TimeStamp;
  int16_t AngularVelocityX;
  int16_t AngularVelocityZ;
  int16_t AngularVelocityY;
  int16_t AccelerometerX;
  int16_t AccelerometerY;
  int16_t AccelerometerZ;
  uint32_t SensorTimestamp;
  int8_t Temperature;
  TouchData touchData;
  uint8_t TriggerRightStopLocation : 4;
  uint8_t TriggerRightStatus : 4;
  uint8_t TriggerLeftStopLocation : 4;
  uint8_t TriggerLeftStatus : 4;
  uint32_t HostTimestamp;
  uint8_t TriggerRightEffect : 4;
  uint8_t TriggerLeftEffect : 4;
  uint32_t DeviceTimeStamp;
  uint8_t PowerPercent : 4;
  PowerState powerState : 4;
  uint8_t PluggedHeadphones : 1;
  uint8_t PluggedMic : 1;
  uint8_t MicMuted : 1;
  uint8_t PluggedUsbData : 1;
  uint8_t PluggedUsbPower : 1;
  uint8_t PluggedUnk1 : 3;
  uint8_t PluggedExternalMic : 1;
  uint8_t HapticLowPassFilter : 1;
  uint8_t PluggedUnk3 : 6;
  uint8_t AesCmac[5];
  uint8_t crc[4];  // CRC32 of the entire packet, not just the data section
};

struct BTGetStateData {
  USBGetStateData StateData;
  uint8_t UNK1;
  uint8_t BtCrcFailCount;
  uint8_t Pad[11];
};

struct ReportIn01USB {
  uint8_t ReportID;  // 0x01
  USBGetStateData State;
};

struct ReportIn01BT {
  uint8_t ReportID;  // 0x01
  BTSimpleGetStateData State;
};

struct ReportIn31 {
  union {
    BTCRC<78> CRC;
    struct {
      uint8_t ReportID;    // 0x31
      uint8_t HasHID : 1;  // Present for packets with state data
      uint8_t HasMic : 1;  // Looks mutually exclusive, possible mic data
      uint8_t Unk1 : 2;
      uint8_t SeqNo : 4;  // unclear progression
      BTGetStateData State;
    } Data;
  };
};

struct SetStateData {
  // Report Set Flags
  uint8_t EnableRumbleEmulation : 1;  // Suggest halving rumble strength
  uint8_t UseRumbleNotHaptics : 1;
  uint8_t AllowRightTriggerFFB : 1;  // Enable setting RightTriggerFFB
  uint8_t AllowLeftTriggerFFB : 1;   // Enable setting LeftTriggerFFB
  uint8_t AllowHeadphoneVolume : 1;  // Enable setting VolumeHeadphones
  uint8_t AllowSpeakerVolume : 1;    // Enable setting VolumeSpeaker
  uint8_t AllowMicVolume : 1;        // Enable setting VolumeMic
  uint8_t AllowAudioControl : 1;     // Enable setting AudioControl section
  uint8_t AllowMuteLight : 1;        // Enable setting MuteLightMode
  uint8_t AllowAudioMute : 1;        // Enable setting MuteControl section
  uint8_t AllowLedColor : 1;         // Enable RGB LED section
  uint8_t ResetLights : 1;  // Release the LEDs from Wireless firmware control
  uint8_t AllowPlayerIndicators : 1;  // Enable setting PlayerIndicators section
  uint8_t AllowHapticLowPassFilter : 1;  // Enable HapticLowPassFilter
  uint8_t AllowMotorPowerLevel : 1;      // MotorPowerLevel reductions for
                                         // trigger/haptic
  uint8_t AllowAudioControl2 : 1;        // Enable setting AudioControl2 section

  uint8_t RumbleEmulationRight;  // emulates the light weight
  uint8_t RumbleEmulationLeft;   // emulated the heavy weight

  uint8_t VolumeHeadphones;  // max 0x7f
  uint8_t VolumeSpeaker;     // PS5 appears to only use the range 0x3d-0x64
  uint8_t VolumeMic;  // not linear, seems to max at 64, 0 is not fully muted

  // AudioControl
  uint8_t MicSelect : 2;  // 0 Auto, 1 Internal Only, 2 External Only, 3 Unclear
  uint8_t EchoCancelEnable : 1;
  uint8_t NoiseCancelEnable : 1;
  uint8_t OutputPathSelect : 2;  // 0 L_R_X, 1 L_L_X, 2 L_L_R, 3 X_X_R
  uint8_t InputPathSelect : 2;   // 0 CHAT_ASR, 1 CHAT_CHAT, 2 ASR_ASR, 3 Does
                                 // Nothing

  MuteLight MuteLightMode;

  // MuteControl
  uint8_t TouchPowerSave : 1;
  uint8_t MotionPowerSave : 1;
  uint8_t HapticPowerSave : 1;  // AKA BulletPowerSave
  uint8_t AudioPowerSave : 1;
  uint8_t MicMute : 1;
  uint8_t SpeakerMute : 1;
  uint8_t HeadphoneMute : 1;
  uint8_t HapticMute : 1;  // AKA BulletMute

  uint8_t RightTriggerFFB[11];
  uint8_t LeftTriggerFFB[11];
  uint32_t HostTimestamp;  // mirrored into report read

  // MotorPowerLevel
  uint8_t TriggerMotorPowerReduction : 4;  // 0x0-0x7 (no 0x8?) Applied
                                           // in 12.5% reductions
  uint8_t RumbleMotorPowerReduction : 4;   // 0x0-0x7 (no 0x8?) Applied
                                           // in 12.5% reductions

  // AudioControl2
  uint8_t SpeakerCompPreGain : 3;  // additional speaker volume boost
  uint8_t BeamformingEnable : 1;   // Probably for MIC given there's 2, might
                                   // be more bits
  uint8_t UnkAudioControl2 : 4;  // some of these bits might apply to the above

  uint8_t AllowLightBrightnessChange : 1;     // LED_BRIGHTNESS_CONTROL
  uint8_t AllowColorLightFadeAnimation : 1;   // LIGHTBAR_SETUP_CONTROL
  uint8_t EnableImprovedRumbleEmulation : 1;  // Use instead of
                                              // EnableRumbleEmulation
  uint8_t UNKBITC : 5;                        // unused

  uint8_t HapticLowPassFilter : 1;
  uint8_t UNKBIT : 7;

  uint8_t UNKBYTE;  // previous notes suggested this was HLPF, was probably
                    // off by 1

  LightFadeAnimation lightFadeAnimation;
  LightBrightness lightBrightness;

  // PlayerIndicators
  uint8_t PlayerLight1 : 1;     // x --- -
  uint8_t PlayerLight2 : 1;     // - x-- -
  uint8_t PlayerLight3 : 1;     // - -x- -
  uint8_t PlayerLight4 : 1;     // - --x -
  uint8_t PlayerLight5 : 1;     // - --- x
  uint8_t PlayerLightFade : 1;  // if low player lights fade in, if high
                                // player lights instantly change
  uint8_t PlayerLightUNK : 2;

  // RGB LED
  uint8_t LedRed;
  uint8_t LedGreen;
  uint8_t LedBlue;
};

struct ReportOut31 {
  union {
    BTCRC<78> CRC;
    struct {
      uint8_t ReportID;       // 0x31
      uint8_t UNK1 : 1;       // -+
      uint8_t EnableHID : 1;  //  | - these 3 bits seem to act as an enum
      uint8_t UNK2 : 1;       // -+
      uint8_t UNK3 : 1;
      uint8_t SeqNo : 4;  // increment for every write // we have no proof of
                          // this, need to see some PS5 captures
      SetStateData State;
    } Data;
  };
};

struct ReportFeatureCalibrationData {
  union {
    BTCRC<41> CRC;
    struct {
      uint8_t ReportID;  // 0x05 // does this exist on USB? confirm
      struct {
        uint16_t pitch_bias;
        uint16_t yaw_bias;
        uint16_t roll_bias;
        uint16_t pitch_plus;
        uint16_t pitch_minus;
        uint16_t yaw_plus;
        uint16_t yaw_minus;
        uint16_t roll_plus;
        uint16_t roll_minus;
        uint16_t speed_plus;
        uint16_t speed_minus;
      } gyro;
      struct {
        uint16_t x_plus;
        uint16_t x_minus;
        uint16_t y_plus;
        uint16_t y_minus;
        uint16_t z_plus;
        uint16_t z_minus;
      } acc;
      int16_t Unknown;
    } Data;
  };
};

struct ReportFeatureInMacAll {
  uint8_t ReportID;      // 0x09
  uint8_t ClientMac[6];  // Right to Left
  uint8_t Hard08;
  uint8_t Hard25;
  uint8_t Hard00;
  uint8_t HostMac[6];  // Right to Left
  uint8_t Pad[3];      // Size according to Linux driver
};

struct ReportFeatureInVersion {
  union {
    BTCRC<64> CRC;
    struct {
      uint8_t ReportID;    // 0x20
      char BuildDate[11];  // string
      char BuildTime[8];   // string
      uint16_t FwType;
      uint16_t SwSeries;
      uint32_t HardwareInfo;  // 0x00FF0000 - Variation
      // 0x0000FF00 - Generation
      // 0x0000003F - Trial?
      // ^ Values tied to enumerations
      uint32_t FirmwareVersion;  // 0xAABBCCCC AA.BB.CCCC
      char DeviceInfo[12];
      ////
      uint16_t UpdateVersion;
      char UpdateImageInfo;
      char UpdateUnk;
      ////
      uint32_t SblFwVersion;
      // 0xAABBCCCC AA.BB.CCCC
      // Ignored for FwType 0
      // HardwareVersion used for FwType 1
      // Unknown behavior if HardwareVersion < 0.1.38 for FwType 2 & 3
      // If HardwareVersion >= 0.1.38 for FwType 2 & 3
      uint32_t VenomFwVersion;
      uint32_t SpiderDspFwVersion;
    } Data;
  };
};
#pragma pack(pop)

#endif  // SRC_BLUEZ_PS5_DUAL_SENSE_DUAL_SENSE_0CE6_H_