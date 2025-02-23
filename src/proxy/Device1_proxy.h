
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_Device1_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_Device1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class Device1_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "org.bluez.Device1";

 protected:
  Device1_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Device1_proxy(const Device1_proxy&) = delete;
  Device1_proxy& operator=(const Device1_proxy&) = delete;
  Device1_proxy(Device1_proxy&&) = delete;
  Device1_proxy& operator=(Device1_proxy&&) = delete;

  ~Device1_proxy() = default;

  void registerProxy() {}

 public:
  void Connect() { m_proxy.callMethod("Connect").onInterface(INTERFACE_NAME); }

  void Disconnect() {
    m_proxy.callMethod("Disconnect").onInterface(INTERFACE_NAME);
  }

  void ConnectProfile(const std::string& uuid) {
    m_proxy.callMethod("ConnectProfile")
        .onInterface(INTERFACE_NAME)
        .withArguments(uuid);
  }

  void DisconnectProfile(const std::string& uuid) {
    m_proxy.callMethod("DisconnectProfile")
        .onInterface(INTERFACE_NAME)
        .withArguments(uuid);
  }

  void Pair() { m_proxy.callMethod("Pair").onInterface(INTERFACE_NAME); }

  void CancelPairing() {
    m_proxy.callMethod("CancelPairing").onInterface(INTERFACE_NAME);
  }

 public:
  std::string Address() {
    return m_proxy.getProperty("Address")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string AddressType() {
    return m_proxy.getProperty("AddressType")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string Name() {
    return m_proxy.getProperty("Name")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string Icon() {
    return m_proxy.getProperty("Icon")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  uint32_t Class() {
    return m_proxy.getProperty("Class")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  uint16_t Appearance() {
    return m_proxy.getProperty("Appearance")
        .onInterface(INTERFACE_NAME)
        .get<uint16_t>();
  }

  std::vector<std::string> UUIDs() {
    return m_proxy.getProperty("UUIDs")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  bool Paired() {
    return m_proxy.getProperty("Paired")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool Connected() {
    return m_proxy.getProperty("Connected")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool Trusted() {
    return m_proxy.getProperty("Trusted")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void Trusted(const bool& value) {
    m_proxy.setProperty("Trusted").onInterface(INTERFACE_NAME).toValue(value);
  }

  bool Blocked() {
    return m_proxy.getProperty("Blocked")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void Blocked(const bool& value) {
    m_proxy.setProperty("Blocked").onInterface(INTERFACE_NAME).toValue(value);
  }

  bool WakeAllowed() {
    return m_proxy.getProperty("WakeAllowed")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void WakeAllowed(const bool& value) {
    m_proxy.setProperty("WakeAllowed")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  std::string Alias() {
    return m_proxy.getProperty("Alias")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  void Alias(const std::string& value) {
    m_proxy.setProperty("Alias").onInterface(INTERFACE_NAME).toValue(value);
  }

  sdbus::ObjectPath Adapter() {
    return m_proxy.getProperty("Adapter")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::ObjectPath>();
  }

  bool LegacyPairing() {
    return m_proxy.getProperty("LegacyPairing")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  std::string Modalias() {
    return m_proxy.getProperty("Modalias")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  int16_t RSSI() {
    return m_proxy.getProperty("RSSI")
        .onInterface(INTERFACE_NAME)
        .get<int16_t>();
  }

  int16_t TxPower() {
    return m_proxy.getProperty("TxPower")
        .onInterface(INTERFACE_NAME)
        .get<int16_t>();
  }

  std::map<uint16_t, sdbus::Variant> ManufacturerData() {
    return m_proxy.getProperty("ManufacturerData")
        .onInterface(INTERFACE_NAME)
        .get<std::map<uint16_t, sdbus::Variant>>();
  }

  std::map<std::string, sdbus::Variant> ServiceData() {
    return m_proxy.getProperty("ServiceData")
        .onInterface(INTERFACE_NAME)
        .get<std::map<std::string, sdbus::Variant>>();
  }

  bool ServicesResolved() {
    return m_proxy.getProperty("ServicesResolved")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  std::vector<bool> AdvertisingFlags() {
    return m_proxy.getProperty("AdvertisingFlags")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<bool>>();
  }

  std::map<uint8_t, sdbus::Variant> AdvertisingData() {
    return m_proxy.getProperty("AdvertisingData")
        .onInterface(INTERFACE_NAME)
        .get<std::map<uint8_t, sdbus::Variant>>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace bluez
}  // namespace org

#endif
