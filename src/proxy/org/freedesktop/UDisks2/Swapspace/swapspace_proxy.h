
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_UDisks2_Swapspace_swapspace_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_UDisks2_Swapspace_swapspace_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace UDisks2 {

class Swapspace_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.UDisks2.Swapspace";

 protected:
  Swapspace_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Swapspace_proxy(const Swapspace_proxy&) = delete;
  Swapspace_proxy& operator=(const Swapspace_proxy&) = delete;
  Swapspace_proxy(Swapspace_proxy&&) = delete;
  Swapspace_proxy& operator=(Swapspace_proxy&&) = delete;

  ~Swapspace_proxy() = default;

  void registerProxy() {}

 public:
  void Start(const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("Start")
        .onInterface(INTERFACE_NAME)
        .withArguments(options);
  }

  void Stop(const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("Stop")
        .onInterface(INTERFACE_NAME)
        .withArguments(options);
  }

  void SetLabel(const std::string& label,
                const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("SetLabel")
        .onInterface(INTERFACE_NAME)
        .withArguments(label, options);
  }

  void SetUUID(const std::string& uuid,
               const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("SetUUID")
        .onInterface(INTERFACE_NAME)
        .withArguments(uuid, options);
  }

 public:
  bool Active() {
    return m_proxy.getProperty("Active")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace UDisks2
}  // namespace freedesktop
}  // namespace org

#endif
