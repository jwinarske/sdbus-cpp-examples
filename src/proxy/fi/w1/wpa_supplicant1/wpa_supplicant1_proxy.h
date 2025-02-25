
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_fi_w1_wpa_supplicant1_wpa_supplicant1_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_fi_w1_wpa_supplicant1_wpa_supplicant1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace fi {
namespace w1 {

class wpa_supplicant1_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "fi.w1.wpa_supplicant1";

 protected:
  wpa_supplicant1_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  wpa_supplicant1_proxy(const wpa_supplicant1_proxy&) = delete;
  wpa_supplicant1_proxy& operator=(const wpa_supplicant1_proxy&) = delete;
  wpa_supplicant1_proxy(wpa_supplicant1_proxy&&) = delete;
  wpa_supplicant1_proxy& operator=(wpa_supplicant1_proxy&&) = delete;

  ~wpa_supplicant1_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("InterfaceAdded")
        .onInterface(INTERFACE_NAME)
        .call([this](const sdbus::ObjectPath& path,
                     const std::map<std::string, sdbus::Variant>& properties) {
          this->onInterfaceAdded(path, properties);
        });
    m_proxy.uponSignal("InterfaceRemoved")
        .onInterface(INTERFACE_NAME)
        .call([this](const sdbus::ObjectPath& path) {
          this->onInterfaceRemoved(path);
        });
    m_proxy.uponSignal("PropertiesChanged")
        .onInterface(INTERFACE_NAME)
        .call([this](const std::map<std::string, sdbus::Variant>& properties) {
          this->onPropertiesChanged(properties);
        });
  }

  virtual void onInterfaceAdded(
      const sdbus::ObjectPath& path,
      const std::map<std::string, sdbus::Variant>& properties) = 0;
  virtual void onInterfaceRemoved(const sdbus::ObjectPath& path) = 0;
  virtual void onPropertiesChanged(
      const std::map<std::string, sdbus::Variant>& properties) = 0;

 public:
  sdbus::ObjectPath CreateInterface(
      const std::map<std::string, sdbus::Variant>& args) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("CreateInterface")
        .onInterface(INTERFACE_NAME)
        .withArguments(args)
        .storeResultsTo(result);
    return result;
  }

  void RemoveInterface(const sdbus::ObjectPath& path) {
    m_proxy.callMethod("RemoveInterface")
        .onInterface(INTERFACE_NAME)
        .withArguments(path);
  }

  sdbus::ObjectPath GetInterface(const std::string& ifname) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetInterface")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifname)
        .storeResultsTo(result);
    return result;
  }

  void ExpectDisconnect() {
    m_proxy.callMethod("ExpectDisconnect").onInterface(INTERFACE_NAME);
  }

 public:
  std::string DebugLevel() {
    return m_proxy.getProperty("DebugLevel")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  void DebugLevel(const std::string& value) {
    m_proxy.setProperty("DebugLevel")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool DebugTimestamp() {
    return m_proxy.getProperty("DebugTimestamp")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void DebugTimestamp(const bool& value) {
    m_proxy.setProperty("DebugTimestamp")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool DebugShowKeys() {
    return m_proxy.getProperty("DebugShowKeys")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void DebugShowKeys(const bool& value) {
    m_proxy.setProperty("DebugShowKeys")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  std::vector<sdbus::ObjectPath> Interfaces() {
    return m_proxy.getProperty("Interfaces")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::ObjectPath>>();
  }

  std::vector<std::string> EapMethods() {
    return m_proxy.getProperty("EapMethods")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::vector<std::string> Capabilities() {
    return m_proxy.getProperty("Capabilities")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::vector<uint8_t> WFDIEs() {
    return m_proxy.getProperty("WFDIEs")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<uint8_t>>();
  }

  void WFDIEs(const std::vector<uint8_t>& value) {
    m_proxy.setProperty("WFDIEs").onInterface(INTERFACE_NAME).toValue(value);
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace w1
}  // namespace fi

#endif
