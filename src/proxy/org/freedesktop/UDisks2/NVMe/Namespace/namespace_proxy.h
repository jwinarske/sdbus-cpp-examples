
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_UDisks2_NVMe_Namespace_namespace_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_UDisks2_NVMe_Namespace_namespace_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace UDisks2 {
namespace NVMe {

class Namespace_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.UDisks2.NVMe.Namespace";

 protected:
  Namespace_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Namespace_proxy(const Namespace_proxy&) = delete;
  Namespace_proxy& operator=(const Namespace_proxy&) = delete;
  Namespace_proxy(Namespace_proxy&&) = delete;
  Namespace_proxy& operator=(Namespace_proxy&&) = delete;

  ~Namespace_proxy() = default;

  void registerProxy() {}

 public:
  void FormatNamespace(const std::map<std::string, sdbus::Variant>& options) {
    m_proxy.callMethod("FormatNamespace")
        .onInterface(INTERFACE_NAME)
        .withArguments(options);
  }

 public:
  uint32_t NSID() {
    return m_proxy.getProperty("NSID")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  std::string NGUID() {
    return m_proxy.getProperty("NGUID")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string EUI64() {
    return m_proxy.getProperty("EUI64")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string UUID() {
    return m_proxy.getProperty("UUID")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string WWN() {
    return m_proxy.getProperty("WWN")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::vector<sdbus::Struct<uint16_t, uint16_t, uint8_t>> LBAFormats() {
    return m_proxy.getProperty("LBAFormats")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::Struct<uint16_t, uint16_t, uint8_t>>>();
  }

  sdbus::Struct<uint16_t, uint16_t, uint8_t> FormattedLBASize() {
    return m_proxy.getProperty("FormattedLBASize")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<uint16_t, uint16_t, uint8_t>>();
  }

  uint64_t NamespaceSize() {
    return m_proxy.getProperty("NamespaceSize")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t NamespaceCapacity() {
    return m_proxy.getProperty("NamespaceCapacity")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t NamespaceUtilization() {
    return m_proxy.getProperty("NamespaceUtilization")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  int32_t FormatPercentRemaining() {
    return m_proxy.getProperty("FormatPercentRemaining")
        .onInterface(INTERFACE_NAME)
        .get<int32_t>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace NVMe
}  // namespace UDisks2
}  // namespace freedesktop
}  // namespace org

#endif
