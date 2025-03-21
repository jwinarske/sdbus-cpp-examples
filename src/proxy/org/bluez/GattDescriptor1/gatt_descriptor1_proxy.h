
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_bluez_GattDescriptor1_gatt_descriptor1_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_bluez_GattDescriptor1_gatt_descriptor1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class GattDescriptor1_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "org.bluez.GattDescriptor1";

 protected:
  GattDescriptor1_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  GattDescriptor1_proxy(const GattDescriptor1_proxy&) = delete;
  GattDescriptor1_proxy& operator=(const GattDescriptor1_proxy&) = delete;
  GattDescriptor1_proxy(GattDescriptor1_proxy&&) = delete;
  GattDescriptor1_proxy& operator=(GattDescriptor1_proxy&&) = delete;

  ~GattDescriptor1_proxy() = default;

  void registerProxy() {}

 public:
  std::vector<uint8_t> ReadValue(
      const std::map<std::string, sdbus::Variant>& options) {
    std::vector<uint8_t> result;
    m_proxy.callMethod("ReadValue")
        .onInterface(INTERFACE_NAME)
        .withArguments(options)
        .storeResultsTo(result);
    return result;
  }

  void WriteValue(const std::vector<uint8_t>& value,
                  const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("WriteValue")
        .onInterface(INTERFACE_NAME)
        .withArguments(value, options);
  }

 public:
  std::string UUID() {
    return m_proxy.getProperty("UUID")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  sdbus::ObjectPath Characteristic() {
    return m_proxy.getProperty("Characteristic")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::ObjectPath>();
  }

  std::vector<uint8_t> Value() {
    return m_proxy.getProperty("Value")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<uint8_t>>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace bluez
}  // namespace org

#endif
