
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_GeoClue2_Client_client_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_GeoClue2_Client_client_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace GeoClue2 {

class Client_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.GeoClue2.Client";

 protected:
  Client_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Client_proxy(const Client_proxy&) = delete;
  Client_proxy& operator=(const Client_proxy&) = delete;
  Client_proxy(Client_proxy&&) = delete;
  Client_proxy& operator=(Client_proxy&&) = delete;

  ~Client_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("LocationUpdated")
        .onInterface(INTERFACE_NAME)
        .call([this](const sdbus::ObjectPath& old,
                     const sdbus::ObjectPath& new_) {
          this->onLocationUpdated(old, new_);
        });
  }

  virtual void onLocationUpdated(const sdbus::ObjectPath& old,
                                 const sdbus::ObjectPath& new_) = 0;

 public:
  void Start() { m_proxy.callMethod("Start").onInterface(INTERFACE_NAME); }

  void Stop() { m_proxy.callMethod("Stop").onInterface(INTERFACE_NAME); }

 public:
  sdbus::ObjectPath Location() {
    return m_proxy.getProperty("Location")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::ObjectPath>();
  }

  uint32_t DistanceThreshold() {
    return m_proxy.getProperty("DistanceThreshold")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  void DistanceThreshold(const uint32_t& value) {
    m_proxy.setProperty("DistanceThreshold")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  uint32_t TimeThreshold() {
    return m_proxy.getProperty("TimeThreshold")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  void TimeThreshold(const uint32_t& value) {
    m_proxy.setProperty("TimeThreshold")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  std::string DesktopId() {
    return m_proxy.getProperty("DesktopId")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  void DesktopId(const std::string& value) {
    m_proxy.setProperty("DesktopId").onInterface(INTERFACE_NAME).toValue(value);
  }

  uint32_t RequestedAccuracyLevel() {
    return m_proxy.getProperty("RequestedAccuracyLevel")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  void RequestedAccuracyLevel(const uint32_t& value) {
    m_proxy.setProperty("RequestedAccuracyLevel")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool Active() {
    return m_proxy.getProperty("Active")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace GeoClue2
}  // namespace freedesktop
}  // namespace org

#endif
