
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_resolve1_Manager_manager_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_resolve1_Manager_manager_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace resolve1 {

class Manager_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.resolve1.Manager";

 protected:
  Manager_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Manager_proxy(const Manager_proxy&) = delete;
  Manager_proxy& operator=(const Manager_proxy&) = delete;
  Manager_proxy(Manager_proxy&&) = delete;
  Manager_proxy& operator=(Manager_proxy&&) = delete;

  ~Manager_proxy() = default;

  void registerProxy() {}

 public:
  std::tuple<std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>,
             std::string,
             uint64_t>
  ResolveHostname(const int32_t& ifindex,
                  const std::string& name,
                  const int32_t& family,
                  const uint64_t& flags) {
    std::tuple<
        std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>,
        std::string, uint64_t>
        result;
    m_proxy.callMethod("ResolveHostname")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, name, family, flags)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<std::vector<sdbus::Struct<int32_t, std::string>>, uint64_t>
  ResolveAddress(const int32_t& ifindex,
                 const int32_t& family,
                 const std::vector<uint8_t>& address,
                 const uint64_t& flags) {
    std::tuple<std::vector<sdbus::Struct<int32_t, std::string>>, uint64_t>
        result;
    m_proxy.callMethod("ResolveAddress")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, family, address, flags)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<
      std::vector<
          sdbus::Struct<int32_t, uint16_t, uint16_t, std::vector<uint8_t>>>,
      uint64_t>
  ResolveRecord(const int32_t& ifindex,
                const std::string& name,
                const uint16_t& class_,
                const uint16_t& type,
                const uint64_t& flags) {
    std::tuple<std::vector<sdbus::Struct<int32_t, uint16_t, uint16_t,
                                         std::vector<uint8_t>>>,
               uint64_t>
        result;
    m_proxy.callMethod("ResolveRecord")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, name, class_, type, flags)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<
      std::vector<sdbus::Struct<
          uint16_t,
          uint16_t,
          uint16_t,
          std::string,
          std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>,
          std::string>>,
      std::vector<std::vector<uint8_t>>,
      std::string,
      std::string,
      std::string,
      uint64_t>
  ResolveService(const int32_t& ifindex,
                 const std::string& name,
                 const std::string& type,
                 const std::string& domain,
                 const int32_t& family,
                 const uint64_t& flags) {
    std::tuple<
        std::vector<sdbus::Struct<
            uint16_t, uint16_t, uint16_t, std::string,
            std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>,
            std::string>>,
        std::vector<std::vector<uint8_t>>, std::string, std::string,
        std::string, uint64_t>
        result;
    m_proxy.callMethod("ResolveService")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, name, type, domain, family, flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath GetLink(const int32_t& ifindex) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetLink")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex)
        .storeResultsTo(result);
    return result;
  }

  void SetLinkDNS(
      const int32_t& ifindex,
      const std::vector<sdbus::Struct<int32_t, std::vector<uint8_t>>>&
          addresses) {
    m_proxy.callMethod("SetLinkDNS")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, addresses);
  }

  void SetLinkDNSEx(
      const int32_t& ifindex,
      const std::vector<
          sdbus::Struct<int32_t, std::vector<uint8_t>, uint16_t, std::string>>&
          addresses) {
    m_proxy.callMethod("SetLinkDNSEx")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, addresses);
  }

  void SetLinkDomains(
      const int32_t& ifindex,
      const std::vector<sdbus::Struct<std::string, bool>>& domains) {
    m_proxy.callMethod("SetLinkDomains")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, domains);
  }

  void SetLinkDefaultRoute(const int32_t& ifindex, const bool& enable) {
    m_proxy.callMethod("SetLinkDefaultRoute")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, enable);
  }

  void SetLinkLLMNR(const int32_t& ifindex, const std::string& mode) {
    m_proxy.callMethod("SetLinkLLMNR")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, mode);
  }

  void SetLinkMulticastDNS(const int32_t& ifindex, const std::string& mode) {
    m_proxy.callMethod("SetLinkMulticastDNS")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, mode);
  }

  void SetLinkDNSOverTLS(const int32_t& ifindex, const std::string& mode) {
    m_proxy.callMethod("SetLinkDNSOverTLS")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, mode);
  }

  void SetLinkDNSSEC(const int32_t& ifindex, const std::string& mode) {
    m_proxy.callMethod("SetLinkDNSSEC")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, mode);
  }

  void SetLinkDNSSECNegativeTrustAnchors(
      const int32_t& ifindex,
      const std::vector<std::string>& names) {
    m_proxy.callMethod("SetLinkDNSSECNegativeTrustAnchors")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex, names);
  }

  void RevertLink(const int32_t& ifindex) {
    m_proxy.callMethod("RevertLink")
        .onInterface(INTERFACE_NAME)
        .withArguments(ifindex);
  }

  sdbus::ObjectPath RegisterService(
      const std::string& name,
      const std::string& name_template,
      const std::string& type,
      const uint16_t& service_port,
      const uint16_t& service_priority,
      const uint16_t& service_weight,
      const std::vector<std::map<std::string, std::vector<uint8_t>>>&
          txt_datas) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("RegisterService")
        .onInterface(INTERFACE_NAME)
        .withArguments(name, name_template, type, service_port,
                       service_priority, service_weight, txt_datas)
        .storeResultsTo(result);
    return result;
  }

  void UnregisterService(const sdbus::ObjectPath& service_path) {
    m_proxy.callMethod("UnregisterService")
        .onInterface(INTERFACE_NAME)
        .withArguments(service_path);
  }

  void ResetStatistics() {
    m_proxy.callMethod("ResetStatistics").onInterface(INTERFACE_NAME);
  }

  void FlushCaches() {
    m_proxy.callMethod("FlushCaches").onInterface(INTERFACE_NAME);
  }

  void ResetServerFeatures() {
    m_proxy.callMethod("ResetServerFeatures").onInterface(INTERFACE_NAME);
  }

 public:
  std::string LLMNRHostname() {
    return m_proxy.getProperty("LLMNRHostname")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string LLMNR() {
    return m_proxy.getProperty("LLMNR")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string MulticastDNS() {
    return m_proxy.getProperty("MulticastDNS")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string DNSOverTLS() {
    return m_proxy.getProperty("DNSOverTLS")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>> DNS() {
    return m_proxy.getProperty("DNS")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<
            sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>>();
  }

  std::vector<
      sdbus::
          Struct<int32_t, int32_t, std::vector<uint8_t>, uint16_t, std::string>>
  DNSEx() {
    return m_proxy.getProperty("DNSEx")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>,
                                       uint16_t, std::string>>>();
  }

  std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>
  FallbackDNS() {
    return m_proxy.getProperty("FallbackDNS")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<
            sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>>();
  }

  std::vector<
      sdbus::
          Struct<int32_t, int32_t, std::vector<uint8_t>, uint16_t, std::string>>
  FallbackDNSEx() {
    return m_proxy.getProperty("FallbackDNSEx")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>,
                                       uint16_t, std::string>>>();
  }

  sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>> CurrentDNSServer() {
    return m_proxy.getProperty("CurrentDNSServer")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>>>();
  }

  sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>, uint16_t, std::string>
  CurrentDNSServerEx() {
    return m_proxy.getProperty("CurrentDNSServerEx")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<int32_t, int32_t, std::vector<uint8_t>, uint16_t,
                           std::string>>();
  }

  std::vector<sdbus::Struct<int32_t, std::string, bool>> Domains() {
    return m_proxy.getProperty("Domains")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::Struct<int32_t, std::string, bool>>>();
  }

  sdbus::Struct<uint64_t, uint64_t> TransactionStatistics() {
    return m_proxy.getProperty("TransactionStatistics")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<uint64_t, uint64_t>>();
  }

  sdbus::Struct<uint64_t, uint64_t, uint64_t> CacheStatistics() {
    return m_proxy.getProperty("CacheStatistics")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<uint64_t, uint64_t, uint64_t>>();
  }

  std::string DNSSEC() {
    return m_proxy.getProperty("DNSSEC")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  sdbus::Struct<uint64_t, uint64_t, uint64_t, uint64_t> DNSSECStatistics() {
    return m_proxy.getProperty("DNSSECStatistics")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<uint64_t, uint64_t, uint64_t, uint64_t>>();
  }

  bool DNSSECSupported() {
    return m_proxy.getProperty("DNSSECSupported")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  std::vector<std::string> DNSSECNegativeTrustAnchors() {
    return m_proxy.getProperty("DNSSECNegativeTrustAnchors")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::string DNSStubListener() {
    return m_proxy.getProperty("DNSStubListener")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string ResolvConfMode() {
    return m_proxy.getProperty("ResolvConfMode")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace resolve1
}  // namespace freedesktop
}  // namespace org

#endif
