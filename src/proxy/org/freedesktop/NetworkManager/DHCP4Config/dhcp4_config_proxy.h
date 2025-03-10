
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_NetworkManager_DHCP4Config_dhcp4_config_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_NetworkManager_DHCP4Config_dhcp4_config_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace NetworkManager {

class DHCP4Config_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.NetworkManager.DHCP4Config";

 protected:
  DHCP4Config_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  DHCP4Config_proxy(const DHCP4Config_proxy&) = delete;
  DHCP4Config_proxy& operator=(const DHCP4Config_proxy&) = delete;
  DHCP4Config_proxy(DHCP4Config_proxy&&) = delete;
  DHCP4Config_proxy& operator=(DHCP4Config_proxy&&) = delete;

  ~DHCP4Config_proxy() = default;

  void registerProxy() {}

 public:
  std::map<std::string, sdbus::Variant> Options() {
    return m_proxy.getProperty("Options")
        .onInterface(INTERFACE_NAME)
        .get<std::map<std::string, sdbus::Variant>>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace NetworkManager
}  // namespace freedesktop
}  // namespace org

#endif
