
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_GeoClue2_Client_Location_location_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_GeoClue2_Client_Location_location_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace GeoClue2 {

class Location_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.GeoClue2.Location";

 protected:
  Location_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Location_proxy(const Location_proxy&) = delete;
  Location_proxy& operator=(const Location_proxy&) = delete;
  Location_proxy(Location_proxy&&) = delete;
  Location_proxy& operator=(Location_proxy&&) = delete;

  ~Location_proxy() = default;

  void registerProxy() {}

 public:
  double Latitude() {
    return m_proxy.getProperty("Latitude")
        .onInterface(INTERFACE_NAME)
        .get<double>();
  }

  double Longitude() {
    return m_proxy.getProperty("Longitude")
        .onInterface(INTERFACE_NAME)
        .get<double>();
  }

  double Accuracy() {
    return m_proxy.getProperty("Accuracy")
        .onInterface(INTERFACE_NAME)
        .get<double>();
  }

  double Altitude() {
    return m_proxy.getProperty("Altitude")
        .onInterface(INTERFACE_NAME)
        .get<double>();
  }

  double Speed() {
    return m_proxy.getProperty("Speed")
        .onInterface(INTERFACE_NAME)
        .get<double>();
  }

  double Heading() {
    return m_proxy.getProperty("Heading")
        .onInterface(INTERFACE_NAME)
        .get<double>();
  }

  std::string Description() {
    return m_proxy.getProperty("Description")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  sdbus::Struct<uint64_t, uint64_t> Timestamp() {
    return m_proxy.getProperty("Timestamp")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<uint64_t, uint64_t>>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace GeoClue2
}  // namespace freedesktop
}  // namespace org

#endif
