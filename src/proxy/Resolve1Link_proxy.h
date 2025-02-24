
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_Resolve1Link_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_Resolve1Link_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace resolve1 {

class Link_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.freedesktop.resolve1.Link";

protected:
    Link_proxy(sdbus::IProxy& proxy)
        : m_proxy(proxy)
    {
    }

    Link_proxy(const Link_proxy&) = delete;
    Link_proxy& operator=(const Link_proxy&) = delete;
    Link_proxy(Link_proxy&&) = delete;
    Link_proxy& operator=(Link_proxy&&) = delete;

    ~Link_proxy() = default;

    void registerProxy()
    {
    }

public:
    void SetDNS(const std::vector<sdbus::Struct<int32_t, std::vector<uint8_t>>>& addresses)
    {
        m_proxy.callMethod("SetDNS").onInterface(INTERFACE_NAME).withArguments(addresses);
    }

    void SetDNSEx(const std::vector<sdbus::Struct<int32_t, std::vector<uint8_t>, uint16_t, std::string>>& addresses)
    {
        m_proxy.callMethod("SetDNSEx").onInterface(INTERFACE_NAME).withArguments(addresses);
    }

    void SetDomains(const std::vector<sdbus::Struct<std::string, bool>>& domains)
    {
        m_proxy.callMethod("SetDomains").onInterface(INTERFACE_NAME).withArguments(domains);
    }

    void SetDefaultRoute(const bool& enable)
    {
        m_proxy.callMethod("SetDefaultRoute").onInterface(INTERFACE_NAME).withArguments(enable);
    }

    void SetLLMNR(const std::string& mode)
    {
        m_proxy.callMethod("SetLLMNR").onInterface(INTERFACE_NAME).withArguments(mode);
    }

    void SetMulticastDNS(const std::string& mode)
    {
        m_proxy.callMethod("SetMulticastDNS").onInterface(INTERFACE_NAME).withArguments(mode);
    }

    void SetDNSOverTLS(const std::string& mode)
    {
        m_proxy.callMethod("SetDNSOverTLS").onInterface(INTERFACE_NAME).withArguments(mode);
    }

    void SetDNSSEC(const std::string& mode)
    {
        m_proxy.callMethod("SetDNSSEC").onInterface(INTERFACE_NAME).withArguments(mode);
    }

    void SetDNSSECNegativeTrustAnchors(const std::vector<std::string>& names)
    {
        m_proxy.callMethod("SetDNSSECNegativeTrustAnchors").onInterface(INTERFACE_NAME).withArguments(names);
    }

    void Revert()
    {
        m_proxy.callMethod("Revert").onInterface(INTERFACE_NAME);
    }

public:
    uint64_t ScopesMask()
    {
        return m_proxy.getProperty("ScopesMask").onInterface(INTERFACE_NAME).get<uint64_t>();
    }

    std::vector<sdbus::Struct<int32_t, std::vector<uint8_t>>> DNS()
    {
        return m_proxy.getProperty("DNS").onInterface(INTERFACE_NAME).get<std::vector<sdbus::Struct<int32_t, std::vector<uint8_t>>>>();
    }

    std::vector<sdbus::Struct<int32_t, std::vector<uint8_t>, uint16_t, std::string>> DNSEx()
    {
        return m_proxy.getProperty("DNSEx").onInterface(INTERFACE_NAME).get<std::vector<sdbus::Struct<int32_t, std::vector<uint8_t>, uint16_t, std::string>>>();
    }

    sdbus::Struct<int32_t, std::vector<uint8_t>> CurrentDNSServer()
    {
        return m_proxy.getProperty("CurrentDNSServer").onInterface(INTERFACE_NAME).get<sdbus::Struct<int32_t, std::vector<uint8_t>>>();
    }

    sdbus::Struct<int32_t, std::vector<uint8_t>, uint16_t, std::string> CurrentDNSServerEx()
    {
        return m_proxy.getProperty("CurrentDNSServerEx").onInterface(INTERFACE_NAME).get<sdbus::Struct<int32_t, std::vector<uint8_t>, uint16_t, std::string>>();
    }

    std::vector<sdbus::Struct<std::string, bool>> Domains()
    {
        return m_proxy.getProperty("Domains").onInterface(INTERFACE_NAME).get<std::vector<sdbus::Struct<std::string, bool>>>();
    }

    bool DefaultRoute()
    {
        return m_proxy.getProperty("DefaultRoute").onInterface(INTERFACE_NAME).get<bool>();
    }

    std::string LLMNR()
    {
        return m_proxy.getProperty("LLMNR").onInterface(INTERFACE_NAME).get<std::string>();
    }

    std::string MulticastDNS()
    {
        return m_proxy.getProperty("MulticastDNS").onInterface(INTERFACE_NAME).get<std::string>();
    }

    std::string DNSOverTLS()
    {
        return m_proxy.getProperty("DNSOverTLS").onInterface(INTERFACE_NAME).get<std::string>();
    }

    std::string DNSSEC()
    {
        return m_proxy.getProperty("DNSSEC").onInterface(INTERFACE_NAME).get<std::string>();
    }

    std::vector<std::string> DNSSECNegativeTrustAnchors()
    {
        return m_proxy.getProperty("DNSSECNegativeTrustAnchors").onInterface(INTERFACE_NAME).get<std::vector<std::string>>();
    }

    bool DNSSECSupported()
    {
        return m_proxy.getProperty("DNSSECSupported").onInterface(INTERFACE_NAME).get<bool>();
    }

private:
    sdbus::IProxy& m_proxy;
};

}}} // namespaces

#endif
