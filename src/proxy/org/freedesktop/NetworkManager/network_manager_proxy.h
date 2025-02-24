
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_NetworkManager_network_manager_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_NetworkManager_network_manager_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {

class NetworkManager_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.NetworkManager";

 protected:
  NetworkManager_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  NetworkManager_proxy(const NetworkManager_proxy&) = delete;
  NetworkManager_proxy& operator=(const NetworkManager_proxy&) = delete;
  NetworkManager_proxy(NetworkManager_proxy&&) = delete;
  NetworkManager_proxy& operator=(NetworkManager_proxy&&) = delete;

  ~NetworkManager_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("PropertiesChanged")
        .onInterface(INTERFACE_NAME)
        .call([this](const std::map<std::string, sdbus::Variant>& properties) {
          this->onPropertiesChanged(properties);
        });
    m_proxy.uponSignal("CheckPermissions")
        .onInterface(INTERFACE_NAME)
        .call([this]() { this->onCheckPermissions(); });
    m_proxy.uponSignal("StateChanged")
        .onInterface(INTERFACE_NAME)
        .call([this](const uint32_t& state) { this->onStateChanged(state); });
    m_proxy.uponSignal("DeviceAdded")
        .onInterface(INTERFACE_NAME)
        .call([this](const sdbus::ObjectPath& device_path) {
          this->onDeviceAdded(device_path);
        });
    m_proxy.uponSignal("DeviceRemoved")
        .onInterface(INTERFACE_NAME)
        .call([this](const sdbus::ObjectPath& device_path) {
          this->onDeviceRemoved(device_path);
        });
  }

  virtual void onPropertiesChanged(
      const std::map<std::string, sdbus::Variant>& properties) = 0;
  virtual void onCheckPermissions() = 0;
  virtual void onStateChanged(const uint32_t& state) = 0;
  virtual void onDeviceAdded(const sdbus::ObjectPath& device_path) = 0;
  virtual void onDeviceRemoved(const sdbus::ObjectPath& device_path) = 0;

 public:
  void Reload(const uint32_t& flags) {
    m_proxy.callMethod("Reload")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  std::vector<sdbus::ObjectPath> GetDevices() {
    std::vector<sdbus::ObjectPath> result;
    m_proxy.callMethod("GetDevices")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::vector<sdbus::ObjectPath> GetAllDevices() {
    std::vector<sdbus::ObjectPath> result;
    m_proxy.callMethod("GetAllDevices")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath GetDeviceByIpIface(const std::string& iface) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetDeviceByIpIface")
        .onInterface(INTERFACE_NAME)
        .withArguments(iface)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath ActivateConnection(
      const sdbus::ObjectPath& connection,
      const sdbus::ObjectPath& device,
      const sdbus::ObjectPath& specific_object) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("ActivateConnection")
        .onInterface(INTERFACE_NAME)
        .withArguments(connection, device, specific_object)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<sdbus::ObjectPath, sdbus::ObjectPath> AddAndActivateConnection(
      const std::map<std::string, std::map<std::string, sdbus::Variant>>&
          connection,
      const sdbus::ObjectPath& device,
      const sdbus::ObjectPath& specific_object) {
    std::tuple<sdbus::ObjectPath, sdbus::ObjectPath> result;
    m_proxy.callMethod("AddAndActivateConnection")
        .onInterface(INTERFACE_NAME)
        .withArguments(connection, device, specific_object)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<sdbus::ObjectPath,
             sdbus::ObjectPath,
             std::map<std::string, sdbus::Variant>>
  AddAndActivateConnection2(
      const std::map<std::string, std::map<std::string, sdbus::Variant>>&
          connection,
      const sdbus::ObjectPath& device,
      const sdbus::ObjectPath& specific_object,
      const std::map<std::string, sdbus::Variant>& options) {
    std::tuple<sdbus::ObjectPath, sdbus::ObjectPath,
               std::map<std::string, sdbus::Variant>>
        result;
    m_proxy.callMethod("AddAndActivateConnection2")
        .onInterface(INTERFACE_NAME)
        .withArguments(connection, device, specific_object, options)
        .storeResultsTo(result);
    return result;
  }

  void DeactivateConnection(const sdbus::ObjectPath& active_connection) {
    m_proxy.callMethod("DeactivateConnection")
        .onInterface(INTERFACE_NAME)
        .withArguments(active_connection);
  }

  void Sleep(const bool& sleep) {
    m_proxy.callMethod("Sleep")
        .onInterface(INTERFACE_NAME)
        .withArguments(sleep);
  }

  void Enable(const bool& enable) {
    m_proxy.callMethod("Enable")
        .onInterface(INTERFACE_NAME)
        .withArguments(enable);
  }

  std::map<std::string, std::string> GetPermissions() {
    std::map<std::string, std::string> result;
    m_proxy.callMethod("GetPermissions")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void SetLogging(const std::string& level, const std::string& domains) {
    m_proxy.callMethod("SetLogging")
        .onInterface(INTERFACE_NAME)
        .withArguments(level, domains);
  }

  std::tuple<std::string, std::string> GetLogging() {
    std::tuple<std::string, std::string> result;
    m_proxy.callMethod("GetLogging")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  uint32_t CheckConnectivity() {
    uint32_t result;
    m_proxy.callMethod("CheckConnectivity")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  uint32_t state() {
    uint32_t result;
    m_proxy.callMethod("state")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath CheckpointCreate(
      const std::vector<sdbus::ObjectPath>& devices,
      const uint32_t& rollback_timeout,
      const uint32_t& flags) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("CheckpointCreate")
        .onInterface(INTERFACE_NAME)
        .withArguments(devices, rollback_timeout, flags)
        .storeResultsTo(result);
    return result;
  }

  void CheckpointDestroy(const sdbus::ObjectPath& checkpoint) {
    m_proxy.callMethod("CheckpointDestroy")
        .onInterface(INTERFACE_NAME)
        .withArguments(checkpoint);
  }

  std::map<std::string, uint32_t> CheckpointRollback(
      const sdbus::ObjectPath& checkpoint) {
    std::map<std::string, uint32_t> result;
    m_proxy.callMethod("CheckpointRollback")
        .onInterface(INTERFACE_NAME)
        .withArguments(checkpoint)
        .storeResultsTo(result);
    return result;
  }

  void CheckpointAdjustRollbackTimeout(const sdbus::ObjectPath& checkpoint,
                                       const uint32_t& add_timeout) {
    m_proxy.callMethod("CheckpointAdjustRollbackTimeout")
        .onInterface(INTERFACE_NAME)
        .withArguments(checkpoint, add_timeout);
  }

 public:
  std::vector<sdbus::ObjectPath> Devices() {
    return m_proxy.getProperty("Devices")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::ObjectPath>>();
  }

  std::vector<sdbus::ObjectPath> AllDevices() {
    return m_proxy.getProperty("AllDevices")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::ObjectPath>>();
  }

  std::vector<sdbus::ObjectPath> Checkpoints() {
    return m_proxy.getProperty("Checkpoints")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::ObjectPath>>();
  }

  bool NetworkingEnabled() {
    return m_proxy.getProperty("NetworkingEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool WirelessEnabled() {
    return m_proxy.getProperty("WirelessEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void WirelessEnabled(const bool& value) {
    m_proxy.setProperty("WirelessEnabled")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool WirelessHardwareEnabled() {
    return m_proxy.getProperty("WirelessHardwareEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool WwanEnabled() {
    return m_proxy.getProperty("WwanEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void WwanEnabled(const bool& value) {
    m_proxy.setProperty("WwanEnabled")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool WwanHardwareEnabled() {
    return m_proxy.getProperty("WwanHardwareEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool WimaxEnabled() {
    return m_proxy.getProperty("WimaxEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void WimaxEnabled(const bool& value) {
    m_proxy.setProperty("WimaxEnabled")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  bool WimaxHardwareEnabled() {
    return m_proxy.getProperty("WimaxHardwareEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  std::vector<sdbus::ObjectPath> ActiveConnections() {
    return m_proxy.getProperty("ActiveConnections")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<sdbus::ObjectPath>>();
  }

  sdbus::ObjectPath PrimaryConnection() {
    return m_proxy.getProperty("PrimaryConnection")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::ObjectPath>();
  }

  std::string PrimaryConnectionType() {
    return m_proxy.getProperty("PrimaryConnectionType")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  uint32_t Metered() {
    return m_proxy.getProperty("Metered")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  sdbus::ObjectPath ActivatingConnection() {
    return m_proxy.getProperty("ActivatingConnection")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::ObjectPath>();
  }

  bool Startup() {
    return m_proxy.getProperty("Startup")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  std::string Version() {
    return m_proxy.getProperty("Version")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  uint32_t Capabilities() {
    return m_proxy.getProperty("Capabilities")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  uint32_t State() {
    return m_proxy.getProperty("State")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  uint32_t Connectivity() {
    return m_proxy.getProperty("Connectivity")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  bool ConnectivityCheckAvailable() {
    return m_proxy.getProperty("ConnectivityCheckAvailable")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool ConnectivityCheckEnabled() {
    return m_proxy.getProperty("ConnectivityCheckEnabled")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void ConnectivityCheckEnabled(const bool& value) {
    m_proxy.setProperty("ConnectivityCheckEnabled")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  std::string ConnectivityCheckUri() {
    return m_proxy.getProperty("ConnectivityCheckUri")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::map<std::string, sdbus::Variant> GlobalDnsConfiguration() {
    return m_proxy.getProperty("GlobalDnsConfiguration")
        .onInterface(INTERFACE_NAME)
        .get<std::map<std::string, sdbus::Variant>>();
  }

  void GlobalDnsConfiguration(
      const std::map<std::string, sdbus::Variant>& value) {
    m_proxy.setProperty("GlobalDnsConfiguration")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace freedesktop
}  // namespace org

#endif
