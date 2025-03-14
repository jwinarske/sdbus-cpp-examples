
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_UDisks2_Filesystem_filesystem_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_UDisks2_Filesystem_filesystem_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace UDisks2 {

class Filesystem_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.UDisks2.Filesystem";

 protected:
  Filesystem_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Filesystem_proxy(const Filesystem_proxy&) = delete;
  Filesystem_proxy& operator=(const Filesystem_proxy&) = delete;
  Filesystem_proxy(Filesystem_proxy&&) = delete;
  Filesystem_proxy& operator=(Filesystem_proxy&&) = delete;

  ~Filesystem_proxy() = default;

  void registerProxy() {}

 public:
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

  std::string Mount(const std::map<std::string, sdbus::Variant>& options) {
    std::string result;
    m_proxy.callMethod("Mount")
        .onInterface(INTERFACE_NAME)
        .withArguments(options)
        .storeResultsTo(result);
    return result;
  }

  void Unmount(const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("Unmount")
        .onInterface(INTERFACE_NAME)
        .withArguments(options);
  }

  void Resize(const uint64_t& size,
              const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("Resize")
        .onInterface(INTERFACE_NAME)
        .withArguments(size, options);
  }

  bool Check(const std::map<std::string, sdbus::Variant>& options) {
    bool result;
    m_proxy.callMethod("Check")
        .onInterface(INTERFACE_NAME)
        .withArguments(options)
        .storeResultsTo(result);
    return result;
  }

  bool Repair(const std::map<std::string, sdbus::Variant>& options) {
    bool result;
    m_proxy.callMethod("Repair")
        .onInterface(INTERFACE_NAME)
        .withArguments(options)
        .storeResultsTo(result);
    return result;
  }

  void TakeOwnership(const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("TakeOwnership")
        .onInterface(INTERFACE_NAME)
        .withArguments(options);
  }

 public:
  std::vector<std::vector<uint8_t>> MountPoints() {
    return m_proxy.getProperty("MountPoints")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::vector<uint8_t>>>();
  }

  uint64_t Size() {
    return m_proxy.getProperty("Size")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace UDisks2
}  // namespace freedesktop
}  // namespace org

#endif
