
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_bluez_LEAdvertisingManager1_le_advertising_manager1_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_bluez_LEAdvertisingManager1_le_advertising_manager1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class LEAdvertisingManager1_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.bluez.LEAdvertisingManager1";

 protected:
  LEAdvertisingManager1_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  LEAdvertisingManager1_proxy(const LEAdvertisingManager1_proxy&) = delete;
  LEAdvertisingManager1_proxy& operator=(const LEAdvertisingManager1_proxy&) =
      delete;
  LEAdvertisingManager1_proxy(LEAdvertisingManager1_proxy&&) = delete;
  LEAdvertisingManager1_proxy& operator=(LEAdvertisingManager1_proxy&&) =
      delete;

  ~LEAdvertisingManager1_proxy() = default;

  void registerProxy() {}

 public:
  void RegisterAdvertisement(
      const sdbus::ObjectPath& advertisement,
      const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("RegisterAdvertisement")
        .onInterface(INTERFACE_NAME)
        .withArguments(advertisement, options);
  }

  void UnregisterAdvertisement(const sdbus::ObjectPath& service) {
    m_proxy.callMethod("UnregisterAdvertisement")
        .onInterface(INTERFACE_NAME)
        .withArguments(service);
  }

 public:
  uint8_t ActiveInstances() {
    return m_proxy.getProperty("ActiveInstances")
        .onInterface(INTERFACE_NAME)
        .get<uint8_t>();
  }

  uint8_t SupportedInstances() {
    return m_proxy.getProperty("SupportedInstances")
        .onInterface(INTERFACE_NAME)
        .get<uint8_t>();
  }

  std::vector<std::string> SupportedIncludes() {
    return m_proxy.getProperty("SupportedIncludes")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::vector<std::string> SupportedSecondaryChannels() {
    return m_proxy.getProperty("SupportedSecondaryChannels")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace bluez
}  // namespace org

#endif
