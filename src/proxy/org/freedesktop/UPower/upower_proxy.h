
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_UPower_upower_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_UPower_upower_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {

class UPower_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "org.freedesktop.UPower";

 protected:
  UPower_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  UPower_proxy(const UPower_proxy&) = delete;
  UPower_proxy& operator=(const UPower_proxy&) = delete;
  UPower_proxy(UPower_proxy&&) = delete;
  UPower_proxy& operator=(UPower_proxy&&) = delete;

  ~UPower_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("DeviceAdded")
        .onInterface(INTERFACE_NAME)
        .call([this](const sdbus::ObjectPath& device) {
          this->onDeviceAdded(device);
        });
    m_proxy.uponSignal("DeviceRemoved")
        .onInterface(INTERFACE_NAME)
        .call([this](const sdbus::ObjectPath& device) {
          this->onDeviceRemoved(device);
        });
  }

  virtual void onDeviceAdded(const sdbus::ObjectPath& device) = 0;
  virtual void onDeviceRemoved(const sdbus::ObjectPath& device) = 0;

 public:
  std::vector<sdbus::ObjectPath> EnumerateDevices() {
    std::vector<sdbus::ObjectPath> result;
    m_proxy.callMethod("EnumerateDevices")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath GetDisplayDevice() {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetDisplayDevice")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string GetCriticalAction() {
    std::string result;
    m_proxy.callMethod("GetCriticalAction")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

 public:
  std::string DaemonVersion() {
    return m_proxy.getProperty("DaemonVersion")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  bool OnBattery() {
    return m_proxy.getProperty("OnBattery")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool LidIsClosed() {
    return m_proxy.getProperty("LidIsClosed")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool LidIsPresent() {
    return m_proxy.getProperty("LidIsPresent")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace freedesktop
}  // namespace org

#endif
