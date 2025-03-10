// Copyright (c) 2025 Joel Winarske
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_DUAL_SENSE_H
#define SRC_DUAL_SENSE_H

#include <vector>

#include "../../upower/upower_client.h"
#include "../adapter1.h"
#include "../device1.h"
#include "../input1.h"
#include "udev_monitor.hpp"

class UPowerDisplayDevice;
class DualSense final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy>,
      public UdevMonitor {
 public:
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

#pragma pack(push, 1)

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
    uint32_t UNK_COUNTER;
    int16_t AngularVelocityX;
    int16_t AngularVelocityZ;
    int16_t AngularVelocityY;
    int16_t AccelerometerX;
    int16_t AccelerometerY;
    int16_t AccelerometerZ;
    uint32_t SensorTimestamp;
    int8_t Temperature;
    TouchData touchData;  // Renamed to avoid conflict
    uint8_t TriggerRightStopLocation : 4;
    uint8_t TriggerRightStatus : 4;
    uint8_t TriggerLeftStopLocation : 4;
    uint8_t TriggerLeftStatus : 4;
    uint32_t HostTimestamp;
    uint8_t TriggerRightEffect : 4;
    uint8_t TriggerLeftEffect : 4;
    uint32_t DeviceTimeStamp;
    uint8_t PowerPercent : 4;
    PowerState powerState : 4;  // Renamed to avoid conflict
    uint8_t PluggedHeadphones : 1;
    uint8_t PluggedMic : 1;
    uint8_t MicMuted : 1;
    uint8_t PluggedUsbData : 1;
    uint8_t PluggedUsbPower : 1;
    uint8_t PluggedUnk1 : 3;
    uint8_t PluggedExternalMic : 1;
    uint8_t HapticLowPassFilter : 1;
    uint8_t PluggedUnk3 : 6;
    uint8_t AesCmac[8];
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
    uint8_t
        AllowPlayerIndicators : 1;  // Enable setting PlayerIndicators section
    uint8_t AllowHapticLowPassFilter : 1;  // Enable HapticLowPassFilter
    uint8_t AllowMotorPowerLevel : 1;      // MotorPowerLevel reductions for
                                           // trigger/haptic
    uint8_t AllowAudioControl2 : 1;  // Enable setting AudioControl2 section

    uint8_t RumbleEmulationRight;  // emulates the light weight
    uint8_t RumbleEmulationLeft;   // emulated the heavy weight

    uint8_t VolumeHeadphones;  // max 0x7f
    uint8_t VolumeSpeaker;     // PS5 appears to only use the range 0x3d-0x64
    uint8_t VolumeMic;  // not linear, seems to max at 64, 0 is not fully muted

    // AudioControl
    uint8_t
        MicSelect : 2;  // 0 Auto, 1 Internal Only, 2 External Only, 3 Unclear
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
    uint8_t
        UnkAudioControl2 : 4;  // some of these bits might apply to the above

    uint8_t AllowLightBrightnessChange : 1;     // LED_BRIGHTNESS_CONTROL
    uint8_t AllowColorLightFadeAnimation : 1;   // LIGHTBAR_SETUP_CONTROL
    uint8_t EnableImprovedRumbleEmulation : 1;  // Use instead of
                                                // EnableRumbleEmulation
    uint8_t UNKBITC : 5;                        // unused

    uint8_t HapticLowPassFilter : 1;
    uint8_t UNKBIT : 7;

    uint8_t UNKBYTE;  // previous notes suggested this was HLPF, was probably
                      // off by 1

    LightFadeAnimation lightFadeAnimation;  // Renamed to avoid conflict
    LightBrightness lightBrightness;        // Renamed to avoid conflict

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
        uint32_t FwVersion1;  // AKA SblFwVersion
        // 0xAABBCCCC AA.BB.CCCC
        // Ignored for FwType 0
        // HardwareVersion used for FwType 1
        // Unknown behavior if HardwareVersion < 0.1.38 for FwType 2 & 3
        // If HardwareVersion >= 0.1.38 for FwType 2 & 3
        uint32_t FwVersion2;  // AKA VenomFwVersion
        uint32_t FwVersion3;  // AKA SpiderDspFwVersion AKA BettyFwVer
        // May be Memory Control Unit for Non-Volatile Storage
      } Data;
    };
#pragma pack(pop)
  };

  static void print_state_data(USBGetStateData* state_data);

  explicit DualSense(sdbus::IConnection& connection);

  ~DualSense() override;

  static int GetControllerAndHostMAC(
      int fd,
      ReportFeatureInMacAll& controller_and_host_mac);
  static void PrintControllerAndHostMac(
      ReportFeatureInMacAll const& controller_and_host_mac);

 private:
  const std::string kVendorId = "054C";
  const std::string kProductId = "0CE6";
  const std::string kDeviceId = "0100";

  static constexpr auto INTERFACE_NAME = "org.bluez";
  static constexpr auto PROPERTIES_INTERFACE_NAME =
      "org.freedesktop.DBus.Properties";
  static constexpr auto INTROSPECTABLE_INTERFACE_NAME =
      "org.freedesktop.DBus.Introspectable";

  std::mutex adapters_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Adapter1>> adapters_;

  std::mutex devices_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Device1>> devices_;

  std::mutex input1_mutex_;
  std::map<sdbus::ObjectPath, std::unique_ptr<Input1>> input1_;

  std::mutex hidraw_devices_mutex_;
  std::map<std::string, std::string> hidraw_devices_;

  std::mutex upower_display_devices_mutex_;
  std::map<std::string, std::unique_ptr<UPowerClient>> upower_clients_;

  void onInterfacesAdded(
      const sdbus::ObjectPath& objectPath,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfacesAndProperties) override;

  void onInterfacesRemoved(
      const sdbus::ObjectPath& objectPath,
      const std::vector<sdbus::InterfaceName>& interfaces) override;

  bool get_bt_hidraw_devices();

  bool get_usb_hidraw_devices();

  static std::string convert_mac_to_path(const std::string& mac_address);

  static bool compare_subsystem_device_paths(const std::string& input_path,
                                             const std::string& hidraw_path);

  static std::string create_device_key_from_serial_number(
      const std::string& serial_number);
};

#endif  // SRC_DUAL_SENSE_H
