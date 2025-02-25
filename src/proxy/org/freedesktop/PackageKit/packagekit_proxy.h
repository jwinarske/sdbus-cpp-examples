
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_PackageKit_packagekit_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_PackageKit_packagekit_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {

class PackageKit_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "org.freedesktop.PackageKit";

 protected:
  PackageKit_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  PackageKit_proxy(const PackageKit_proxy&) = delete;
  PackageKit_proxy& operator=(const PackageKit_proxy&) = delete;
  PackageKit_proxy(PackageKit_proxy&&) = delete;
  PackageKit_proxy& operator=(PackageKit_proxy&&) = delete;

  ~PackageKit_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("TransactionListChanged")
        .onInterface(INTERFACE_NAME)
        .call([this](const std::vector<std::string>& transactions) {
          this->onTransactionListChanged(transactions);
        });
    m_proxy.uponSignal("RestartSchedule")
        .onInterface(INTERFACE_NAME)
        .call([this]() { this->onRestartSchedule(); });
    m_proxy.uponSignal("RepoListChanged")
        .onInterface(INTERFACE_NAME)
        .call([this]() { this->onRepoListChanged(); });
    m_proxy.uponSignal("UpdatesChanged")
        .onInterface(INTERFACE_NAME)
        .call([this]() { this->onUpdatesChanged(); });
  }

  virtual void onTransactionListChanged(
      const std::vector<std::string>& transactions) = 0;
  virtual void onRestartSchedule() = 0;
  virtual void onRepoListChanged() = 0;
  virtual void onUpdatesChanged() = 0;

 public:
  uint32_t CanAuthorize(const std::string& action_id) {
    uint32_t result;
    m_proxy.callMethod("CanAuthorize")
        .onInterface(INTERFACE_NAME)
        .withArguments(action_id)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath CreateTransaction() {
    sdbus::ObjectPath result;
    m_proxy.callMethod("CreateTransaction")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  uint32_t GetTimeSinceAction(const uint32_t& role) {
    uint32_t result;
    m_proxy.callMethod("GetTimeSinceAction")
        .onInterface(INTERFACE_NAME)
        .withArguments(role)
        .storeResultsTo(result);
    return result;
  }

  std::vector<sdbus::ObjectPath> GetTransactionList() {
    std::vector<sdbus::ObjectPath> result;
    m_proxy.callMethod("GetTransactionList")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void StateHasChanged(const std::string& reason) {
    m_proxy.callMethod("StateHasChanged")
        .onInterface(INTERFACE_NAME)
        .withArguments(reason);
  }

  void SuggestDaemonQuit() {
    m_proxy.callMethod("SuggestDaemonQuit").onInterface(INTERFACE_NAME);
  }

  std::map<std::string, std::vector<std::map<std::string, sdbus::Variant>>>
  GetPackageHistory(const std::vector<std::string>& names,
                    const uint32_t& count) {
    std::map<std::string, std::vector<std::map<std::string, sdbus::Variant>>>
        result;
    m_proxy.callMethod("GetPackageHistory")
        .onInterface(INTERFACE_NAME)
        .withArguments(names, count)
        .storeResultsTo(result);
    return result;
  }

  std::string GetDaemonState() {
    std::string result;
    m_proxy.callMethod("GetDaemonState")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void SetProxy(const std::string& proxy_http,
                const std::string& proxy_https,
                const std::string& proxy_ftp,
                const std::string& proxy_socks,
                const std::string& no_proxy,
                const std::string& pac) {
    m_proxy.callMethod("SetProxy")
        .onInterface(INTERFACE_NAME)
        .withArguments(proxy_http, proxy_https, proxy_ftp, proxy_socks,
                       no_proxy, pac);
  }

 public:
  uint32_t VersionMajor() {
    return m_proxy.getProperty("VersionMajor")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  uint32_t VersionMinor() {
    return m_proxy.getProperty("VersionMinor")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  uint32_t VersionMicro() {
    return m_proxy.getProperty("VersionMicro")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  std::string BackendName() {
    return m_proxy.getProperty("BackendName")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string BackendDescription() {
    return m_proxy.getProperty("BackendDescription")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string BackendAuthor() {
    return m_proxy.getProperty("BackendAuthor")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  uint64_t Roles() {
    return m_proxy.getProperty("Roles")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t Groups() {
    return m_proxy.getProperty("Groups")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t Filters() {
    return m_proxy.getProperty("Filters")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  std::vector<std::string> MimeTypes() {
    return m_proxy.getProperty("MimeTypes")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  bool Locked() {
    return m_proxy.getProperty("Locked")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  uint32_t NetworkState() {
    return m_proxy.getProperty("NetworkState")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  std::string DistroId() {
    return m_proxy.getProperty("DistroId")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace freedesktop
}  // namespace org

#endif
