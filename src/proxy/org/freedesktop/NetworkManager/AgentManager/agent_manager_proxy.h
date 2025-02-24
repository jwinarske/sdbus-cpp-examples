
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_NetworkManager_AgentManager_agent_manager_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_NetworkManager_AgentManager_agent_manager_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace NetworkManager {

class AgentManager_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.NetworkManager.AgentManager";

 protected:
  AgentManager_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  AgentManager_proxy(const AgentManager_proxy&) = delete;
  AgentManager_proxy& operator=(const AgentManager_proxy&) = delete;
  AgentManager_proxy(AgentManager_proxy&&) = delete;
  AgentManager_proxy& operator=(AgentManager_proxy&&) = delete;

  ~AgentManager_proxy() = default;

  void registerProxy() {}

 public:
  void Register(const std::string& identifier) {
    m_proxy.callMethod("Register")
        .onInterface(INTERFACE_NAME)
        .withArguments(identifier);
  }

  void RegisterWithCapabilities(const std::string& identifier,
                                const uint32_t& capabilities) {
    m_proxy.callMethod("RegisterWithCapabilities")
        .onInterface(INTERFACE_NAME)
        .withArguments(identifier, capabilities);
  }

  void Unregister() {
    m_proxy.callMethod("Unregister").onInterface(INTERFACE_NAME);
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace NetworkManager
}  // namespace freedesktop
}  // namespace org

#endif
