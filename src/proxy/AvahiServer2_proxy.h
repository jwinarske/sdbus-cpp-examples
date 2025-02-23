
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_AvahiServer2_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_AvahiServer2_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace Avahi {

class Server2_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "org.freedesktop.Avahi.Server2";

 protected:
  Server2_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Server2_proxy(const Server2_proxy&) = delete;
  Server2_proxy& operator=(const Server2_proxy&) = delete;
  Server2_proxy(Server2_proxy&&) = delete;
  Server2_proxy& operator=(Server2_proxy&&) = delete;

  ~Server2_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("StateChanged")
        .onInterface(INTERFACE_NAME)
        .call([this](const int32_t& state, const std::string& error) {
          this->onStateChanged(state, error);
        });
  }

  virtual void onStateChanged(const int32_t& state,
                              const std::string& error) = 0;

 public:
  std::string GetVersionString() {
    std::string result;
    m_proxy.callMethod("GetVersionString")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  uint32_t GetAPIVersion() {
    uint32_t result;
    m_proxy.callMethod("GetAPIVersion")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string GetHostName() {
    std::string result;
    m_proxy.callMethod("GetHostName")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void SetHostName(const std::string& name) {
    m_proxy.callMethod("SetHostName")
        .onInterface(INTERFACE_NAME)
        .withArguments(name);
  }

  std::string GetHostNameFqdn() {
    std::string result;
    m_proxy.callMethod("GetHostNameFqdn")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string GetDomainName() {
    std::string result;
    m_proxy.callMethod("GetDomainName")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  bool IsNSSSupportAvailable() {
    bool result;
    m_proxy.callMethod("IsNSSSupportAvailable")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  int32_t GetState() {
    int32_t result;
    m_proxy.callMethod("GetState")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  uint32_t GetLocalServiceCookie() {
    uint32_t result;
    m_proxy.callMethod("GetLocalServiceCookie")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string GetAlternativeHostName(const std::string& name) {
    std::string result;
    m_proxy.callMethod("GetAlternativeHostName")
        .onInterface(INTERFACE_NAME)
        .withArguments(name)
        .storeResultsTo(result);
    return result;
  }

  std::string GetAlternativeServiceName(const std::string& name) {
    std::string result;
    m_proxy.callMethod("GetAlternativeServiceName")
        .onInterface(INTERFACE_NAME)
        .withArguments(name)
        .storeResultsTo(result);
    return result;
  }

  std::string GetNetworkInterfaceNameByIndex(const int32_t& index) {
    std::string result;
    m_proxy.callMethod("GetNetworkInterfaceNameByIndex")
        .onInterface(INTERFACE_NAME)
        .withArguments(index)
        .storeResultsTo(result);
    return result;
  }

  int32_t GetNetworkInterfaceIndexByName(const std::string& name) {
    int32_t result;
    m_proxy.callMethod("GetNetworkInterfaceIndexByName")
        .onInterface(INTERFACE_NAME)
        .withArguments(name)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<int32_t, int32_t, std::string, int32_t, std::string, uint32_t>
  ResolveHostName(const int32_t& interface,
                  const int32_t& protocol,
                  const std::string& name,
                  const int32_t& aprotocol,
                  const uint32_t& flags) {
    std::tuple<int32_t, int32_t, std::string, int32_t, std::string, uint32_t>
        result;
    m_proxy.callMethod("ResolveHostName")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, name, aprotocol, flags)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<int32_t, int32_t, int32_t, std::string, std::string, uint32_t>
  ResolveAddress(const int32_t& interface,
                 const int32_t& protocol,
                 const std::string& address,
                 const uint32_t& flags) {
    std::tuple<int32_t, int32_t, int32_t, std::string, std::string, uint32_t>
        result;
    m_proxy.callMethod("ResolveAddress")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, address, flags)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<int32_t,
             int32_t,
             std::string,
             std::string,
             std::string,
             std::string,
             int32_t,
             std::string,
             uint16_t,
             std::vector<std::vector<uint8_t>>,
             uint32_t>
  ResolveService(const int32_t& interface,
                 const int32_t& protocol,
                 const std::string& name,
                 const std::string& type,
                 const std::string& domain,
                 const int32_t& aprotocol,
                 const uint32_t& flags) {
    std::tuple<int32_t, int32_t, std::string, std::string, std::string,
               std::string, int32_t, std::string, uint16_t,
               std::vector<std::vector<uint8_t>>, uint32_t>
        result;
    m_proxy.callMethod("ResolveService")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, name, type, domain, aprotocol,
                       flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath EntryGroupNew() {
    sdbus::ObjectPath result;
    m_proxy.callMethod("EntryGroupNew")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath DomainBrowserPrepare(const int32_t& interface,
                                         const int32_t& protocol,
                                         const std::string& domain,
                                         const int32_t& btype,
                                         const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("DomainBrowserPrepare")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, domain, btype, flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath ServiceTypeBrowserPrepare(const int32_t& interface,
                                              const int32_t& protocol,
                                              const std::string& domain,
                                              const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("ServiceTypeBrowserPrepare")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, domain, flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath ServiceBrowserPrepare(const int32_t& interface,
                                          const int32_t& protocol,
                                          const std::string& type,
                                          const std::string& domain,
                                          const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("ServiceBrowserPrepare")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, type, domain, flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath ServiceResolverPrepare(const int32_t& interface,
                                           const int32_t& protocol,
                                           const std::string& name,
                                           const std::string& type,
                                           const std::string& domain,
                                           const int32_t& aprotocol,
                                           const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("ServiceResolverPrepare")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, name, type, domain, aprotocol,
                       flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath HostNameResolverPrepare(const int32_t& interface,
                                            const int32_t& protocol,
                                            const std::string& name,
                                            const int32_t& aprotocol,
                                            const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("HostNameResolverPrepare")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, name, aprotocol, flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath AddressResolverPrepare(const int32_t& interface,
                                           const int32_t& protocol,
                                           const std::string& address,
                                           const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("AddressResolverPrepare")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, address, flags)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath RecordBrowserPrepare(const int32_t& interface,
                                         const int32_t& protocol,
                                         const std::string& name,
                                         const uint16_t& clazz,
                                         const uint16_t& type,
                                         const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("RecordBrowserPrepare")
        .onInterface(INTERFACE_NAME)
        .withArguments(interface, protocol, name, clazz, type, flags)
        .storeResultsTo(result);
    return result;
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace Avahi
}  // namespace freedesktop
}  // namespace org

#endif
