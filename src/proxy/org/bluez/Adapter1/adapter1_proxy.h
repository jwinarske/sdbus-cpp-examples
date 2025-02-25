
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_bluez_Adapter1_adapter1_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_bluez_Adapter1_adapter1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class Adapter1_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "org.bluez.Adapter1";

 protected:
  Adapter1_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Adapter1_proxy(const Adapter1_proxy&) = delete;
  Adapter1_proxy& operator=(const Adapter1_proxy&) = delete;
  Adapter1_proxy(Adapter1_proxy&&) = delete;
  Adapter1_proxy& operator=(Adapter1_proxy&&) = delete;

  ~Adapter1_proxy() = default;

  void registerProxy() {}

 public:
  void StartDiscovery() {
    m_proxy.callMethod("StartDiscovery").onInterface(INTERFACE_NAME);
  }

  void SetDiscoveryFilter(
      const std::map<std::string, sdbus::Variant>& properties) {
    m_proxy.callMethod("SetDiscoveryFilter")
        .onInterface(INTERFACE_NAME)
        .withArguments(properties);
  }

  void StopDiscovery() {
    m_proxy.callMethod("StopDiscovery").onInterface(INTERFACE_NAME);
  }

  void RemoveDevice(const sdbus::ObjectPath& device) {
    m_proxy.callMethod("RemoveDevice")
        .onInterface(INTERFACE_NAME)
        .withArguments(device);
  }

  std::vector<std::string> GetDiscoveryFilters() {
    std::vector<std::string> result;
    m_proxy.callMethod("GetDiscoveryFilters")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void ConnectDevice(const std::map<std::string, sdbus::Variant>& properties) {
    m_proxy.callMethod("ConnectDevice")
        .onInterface(INTERFACE_NAME)
        .withArguments(properties);
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

  std::string Alias() {
    return m_proxy.getProperty("Alias")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  void Alias(const std::string& value) {
    m_proxy.setProperty("Alias").onInterface(INTERFACE_NAME).toValue(value);
  }

  uint32_t Class() {
    return m_proxy.getProperty("Class")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  bool Powered() {
    return m_proxy.getProperty("Powered")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void Powered(const bool& value) {
    m_proxy.setProperty("Powered").onInterface(INTERFACE_NAME).toValue(value);
  }

  bool Discoverable() {
    return m_proxy.getProperty("Discoverable")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void Discoverable(const bool& value) {
    m_proxy.setProperty("Discoverable")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  uint32_t DiscoverableTimeout() {
    return m_proxy.getProperty("DiscoverableTimeout")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  void DiscoverableTimeout(const uint32_t& value) {
    m_proxy.setProperty("DiscoverableTimeout")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool Pairable() {
    return m_proxy.getProperty("Pairable")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void Pairable(const bool& value) {
    m_proxy.setProperty("Pairable").onInterface(INTERFACE_NAME).toValue(value);
  }

  uint32_t PairableTimeout() {
    return m_proxy.getProperty("PairableTimeout")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  void PairableTimeout(const uint32_t& value) {
    m_proxy.setProperty("PairableTimeout")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool Discovering() {
    return m_proxy.getProperty("Discovering")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  std::vector<std::string> UUIDs() {
    return m_proxy.getProperty("UUIDs")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::string Modalias() {
    return m_proxy.getProperty("Modalias")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace bluez
}  // namespace org

#endif
