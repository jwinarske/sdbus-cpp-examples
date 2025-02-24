
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_UDisks2_Manager_manager_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_UDisks2_Manager_manager_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace UDisks2 {

class Manager_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.UDisks2.Manager";

 protected:
  Manager_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Manager_proxy(const Manager_proxy&) = delete;
  Manager_proxy& operator=(const Manager_proxy&) = delete;
  Manager_proxy(Manager_proxy&&) = delete;
  Manager_proxy& operator=(Manager_proxy&&) = delete;

  ~Manager_proxy() = default;

  void registerProxy() {}

 public:
  sdbus::Struct<bool, std::string> CanFormat(const std::string& type) {
    sdbus::Struct<bool, std::string> result;
    m_proxy.callMethod("CanFormat")
        .onInterface(INTERFACE_NAME)
        .withArguments(type)
        .storeResultsTo(result);
    return result;
  }

  sdbus::Struct<bool, uint64_t, std::string> CanResize(
      const std::string& type) {
    sdbus::Struct<bool, uint64_t, std::string> result;
    m_proxy.callMethod("CanResize")
        .onInterface(INTERFACE_NAME)
        .withArguments(type)
        .storeResultsTo(result);
    return result;
  }

  sdbus::Struct<bool, std::string> CanCheck(const std::string& type) {
    sdbus::Struct<bool, std::string> result;
    m_proxy.callMethod("CanCheck")
        .onInterface(INTERFACE_NAME)
        .withArguments(type)
        .storeResultsTo(result);
    return result;
  }

  sdbus::Struct<bool, std::string> CanRepair(const std::string& type) {
    sdbus::Struct<bool, std::string> result;
    m_proxy.callMethod("CanRepair")
        .onInterface(INTERFACE_NAME)
        .withArguments(type)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath LoopSetup(
      const sdbus::UnixFd& fd,
      const std::map<std::string, sdbus::Variant>& options) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("LoopSetup")
        .onInterface(INTERFACE_NAME)
        .withArguments(fd, options)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath MDRaidCreate(
      const std::vector<sdbus::ObjectPath>& blocks,
      const std::string& level,
      const std::string& name,
      const uint64_t& chunk,
      const std::map<std::string, sdbus::Variant>& options) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("MDRaidCreate")
        .onInterface(INTERFACE_NAME)
        .withArguments(blocks, level, name, chunk, options)
        .storeResultsTo(result);
    return result;
  }

  void EnableModules(const bool& enable) {
    m_proxy.callMethod("EnableModules")
        .onInterface(INTERFACE_NAME)
        .withArguments(enable);
  }

  void EnableModule(const std::string& name, const bool& enable) {
    m_proxy.callMethod("EnableModule")
        .onInterface(INTERFACE_NAME)
        .withArguments(name, enable);
  }

  std::vector<sdbus::ObjectPath> GetBlockDevices(
      const std::map<std::string, sdbus::Variant>& options) {
    std::vector<sdbus::ObjectPath> result;
    m_proxy.callMethod("GetBlockDevices")
        .onInterface(INTERFACE_NAME)
        .withArguments(options)
        .storeResultsTo(result);
    return result;
  }

  std::vector<sdbus::ObjectPath> ResolveDevice(
      const std::map<std::string, sdbus::Variant>& devspec,
      const std::map<std::string, sdbus::Variant>& options) {
    std::vector<sdbus::ObjectPath> result;
    m_proxy.callMethod("ResolveDevice")
        .onInterface(INTERFACE_NAME)
        .withArguments(devspec, options)
        .storeResultsTo(result);
    return result;
  }

 public:
  std::string Version() {
    return m_proxy.getProperty("Version")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::vector<std::string> SupportedFilesystems() {
    return m_proxy.getProperty("SupportedFilesystems")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::vector<std::string> SupportedEncryptionTypes() {
    return m_proxy.getProperty("SupportedEncryptionTypes")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::string DefaultEncryptionType() {
    return m_proxy.getProperty("DefaultEncryptionType")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace UDisks2
}  // namespace freedesktop
}  // namespace org

#endif
