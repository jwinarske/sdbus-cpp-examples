
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_login1_Manager_manager_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_login1_Manager_manager_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {
namespace login1 {

class Manager_proxy {
 public:
  static constexpr const char* INTERFACE_NAME =
      "org.freedesktop.login1.Manager";

 protected:
  Manager_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  Manager_proxy(const Manager_proxy&) = delete;
  Manager_proxy& operator=(const Manager_proxy&) = delete;
  Manager_proxy(Manager_proxy&&) = delete;
  Manager_proxy& operator=(Manager_proxy&&) = delete;

  ~Manager_proxy() = default;

  void registerProxy() {
    m_proxy.uponSignal("SessionNew")
        .onInterface(INTERFACE_NAME)
        .call([this](const std::string& session_id,
                     const sdbus::ObjectPath& object_path) {
          this->onSessionNew(session_id, object_path);
        });
    m_proxy.uponSignal("SessionRemoved")
        .onInterface(INTERFACE_NAME)
        .call([this](const std::string& session_id,
                     const sdbus::ObjectPath& object_path) {
          this->onSessionRemoved(session_id, object_path);
        });
    m_proxy.uponSignal("UserNew")
        .onInterface(INTERFACE_NAME)
        .call(
            [this](const uint32_t& uid, const sdbus::ObjectPath& object_path) {
              this->onUserNew(uid, object_path);
            });
    m_proxy.uponSignal("UserRemoved")
        .onInterface(INTERFACE_NAME)
        .call(
            [this](const uint32_t& uid, const sdbus::ObjectPath& object_path) {
              this->onUserRemoved(uid, object_path);
            });
    m_proxy.uponSignal("SeatNew")
        .onInterface(INTERFACE_NAME)
        .call([this](const std::string& seat_id,
                     const sdbus::ObjectPath& object_path) {
          this->onSeatNew(seat_id, object_path);
        });
    m_proxy.uponSignal("SeatRemoved")
        .onInterface(INTERFACE_NAME)
        .call([this](const std::string& seat_id,
                     const sdbus::ObjectPath& object_path) {
          this->onSeatRemoved(seat_id, object_path);
        });
    m_proxy.uponSignal("PrepareForShutdown")
        .onInterface(INTERFACE_NAME)
        .call([this](const bool& start) { this->onPrepareForShutdown(start); });
    m_proxy.uponSignal("PrepareForShutdownWithMetadata")
        .onInterface(INTERFACE_NAME)
        .call([this](const bool& start,
                     const std::map<std::string, sdbus::Variant>& metadata) {
          this->onPrepareForShutdownWithMetadata(start, metadata);
        });
    m_proxy.uponSignal("PrepareForSleep")
        .onInterface(INTERFACE_NAME)
        .call([this](const bool& start) { this->onPrepareForSleep(start); });
  }

  virtual void onSessionNew(const std::string& session_id,
                            const sdbus::ObjectPath& object_path) = 0;
  virtual void onSessionRemoved(const std::string& session_id,
                                const sdbus::ObjectPath& object_path) = 0;
  virtual void onUserNew(const uint32_t& uid,
                         const sdbus::ObjectPath& object_path) = 0;
  virtual void onUserRemoved(const uint32_t& uid,
                             const sdbus::ObjectPath& object_path) = 0;
  virtual void onSeatNew(const std::string& seat_id,
                         const sdbus::ObjectPath& object_path) = 0;
  virtual void onSeatRemoved(const std::string& seat_id,
                             const sdbus::ObjectPath& object_path) = 0;
  virtual void onPrepareForShutdown(const bool& start) = 0;
  virtual void onPrepareForShutdownWithMetadata(
      const bool& start,
      const std::map<std::string, sdbus::Variant>& metadata) = 0;
  virtual void onPrepareForSleep(const bool& start) = 0;

 public:
  sdbus::ObjectPath GetSession(const std::string& session_id) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath GetSessionByPID(const uint32_t& pid) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetSessionByPID")
        .onInterface(INTERFACE_NAME)
        .withArguments(pid)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath GetUser(const uint32_t& uid) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetUser")
        .onInterface(INTERFACE_NAME)
        .withArguments(uid)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath GetUserByPID(const uint32_t& pid) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetUserByPID")
        .onInterface(INTERFACE_NAME)
        .withArguments(pid)
        .storeResultsTo(result);
    return result;
  }

  sdbus::ObjectPath GetSeat(const std::string& seat_id) {
    sdbus::ObjectPath result;
    m_proxy.callMethod("GetSeat")
        .onInterface(INTERFACE_NAME)
        .withArguments(seat_id)
        .storeResultsTo(result);
    return result;
  }

  std::vector<sdbus::Struct<std::string,
                            uint32_t,
                            std::string,
                            std::string,
                            sdbus::ObjectPath>>
  ListSessions() {
    std::vector<sdbus::Struct<std::string, uint32_t, std::string, std::string,
                              sdbus::ObjectPath>>
        result;
    m_proxy.callMethod("ListSessions")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::vector<sdbus::Struct<uint32_t, std::string, sdbus::ObjectPath>>
  ListUsers() {
    std::vector<sdbus::Struct<uint32_t, std::string, sdbus::ObjectPath>> result;
    m_proxy.callMethod("ListUsers")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::vector<sdbus::Struct<std::string, sdbus::ObjectPath>> ListSeats() {
    std::vector<sdbus::Struct<std::string, sdbus::ObjectPath>> result;
    m_proxy.callMethod("ListSeats")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::vector<sdbus::Struct<std::string,
                            std::string,
                            std::string,
                            std::string,
                            uint32_t,
                            uint32_t>>
  ListInhibitors() {
    std::vector<sdbus::Struct<std::string, std::string, std::string,
                              std::string, uint32_t, uint32_t>>
        result;
    m_proxy.callMethod("ListInhibitors")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<std::string,
             sdbus::ObjectPath,
             std::string,
             sdbus::UnixFd,
             uint32_t,
             std::string,
             uint32_t,
             bool>
  CreateSession(const uint32_t& uid,
                const uint32_t& pid,
                const std::string& service,
                const std::string& type,
                const std::string& class_,
                const std::string& desktop,
                const std::string& seat_id,
                const uint32_t& vtnr,
                const std::string& tty,
                const std::string& display,
                const bool& remote,
                const std::string& remote_user,
                const std::string& remote_host,
                const std::vector<sdbus::Struct<std::string, sdbus::Variant>>&
                    properties) {
    std::tuple<std::string, sdbus::ObjectPath, std::string, sdbus::UnixFd,
               uint32_t, std::string, uint32_t, bool>
        result;
    m_proxy.callMethod("CreateSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(uid, pid, service, type, class_, desktop, seat_id, vtnr,
                       tty, display, remote, remote_user, remote_host,
                       properties)
        .storeResultsTo(result);
    return result;
  }

  std::tuple<std::string,
             sdbus::ObjectPath,
             std::string,
             sdbus::UnixFd,
             uint32_t,
             std::string,
             uint32_t,
             bool>
  CreateSessionWithPIDFD(
      const uint32_t& uid,
      const sdbus::UnixFd& pidfd,
      const std::string& service,
      const std::string& type,
      const std::string& class_,
      const std::string& desktop,
      const std::string& seat_id,
      const uint32_t& vtnr,
      const std::string& tty,
      const std::string& display,
      const bool& remote,
      const std::string& remote_user,
      const std::string& remote_host,
      const uint64_t& flags,
      const std::vector<sdbus::Struct<std::string, sdbus::Variant>>&
          properties) {
    std::tuple<std::string, sdbus::ObjectPath, std::string, sdbus::UnixFd,
               uint32_t, std::string, uint32_t, bool>
        result;
    m_proxy.callMethod("CreateSessionWithPIDFD")
        .onInterface(INTERFACE_NAME)
        .withArguments(uid, pidfd, service, type, class_, desktop, seat_id,
                       vtnr, tty, display, remote, remote_user, remote_host,
                       flags, properties)
        .storeResultsTo(result);
    return result;
  }

  void ReleaseSession(const std::string& session_id) {
    m_proxy.callMethod("ReleaseSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id);
  }

  void ActivateSession(const std::string& session_id) {
    m_proxy.callMethod("ActivateSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id);
  }

  void ActivateSessionOnSeat(const std::string& session_id,
                             const std::string& seat_id) {
    m_proxy.callMethod("ActivateSessionOnSeat")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id, seat_id);
  }

  void LockSession(const std::string& session_id) {
    m_proxy.callMethod("LockSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id);
  }

  void UnlockSession(const std::string& session_id) {
    m_proxy.callMethod("UnlockSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id);
  }

  void LockSessions() {
    m_proxy.callMethod("LockSessions").onInterface(INTERFACE_NAME);
  }

  void UnlockSessions() {
    m_proxy.callMethod("UnlockSessions").onInterface(INTERFACE_NAME);
  }

  void KillSession(const std::string& session_id,
                   const std::string& who,
                   const int32_t& signal_number) {
    m_proxy.callMethod("KillSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id, who, signal_number);
  }

  void KillUser(const uint32_t& uid, const int32_t& signal_number) {
    m_proxy.callMethod("KillUser")
        .onInterface(INTERFACE_NAME)
        .withArguments(uid, signal_number);
  }

  void TerminateSession(const std::string& session_id) {
    m_proxy.callMethod("TerminateSession")
        .onInterface(INTERFACE_NAME)
        .withArguments(session_id);
  }

  void TerminateUser(const uint32_t& uid) {
    m_proxy.callMethod("TerminateUser")
        .onInterface(INTERFACE_NAME)
        .withArguments(uid);
  }

  void TerminateSeat(const std::string& seat_id) {
    m_proxy.callMethod("TerminateSeat")
        .onInterface(INTERFACE_NAME)
        .withArguments(seat_id);
  }

  void SetUserLinger(const uint32_t& uid,
                     const bool& enable,
                     const bool& interactive) {
    m_proxy.callMethod("SetUserLinger")
        .onInterface(INTERFACE_NAME)
        .withArguments(uid, enable, interactive);
  }

  void AttachDevice(const std::string& seat_id,
                    const std::string& sysfs_path,
                    const bool& interactive) {
    m_proxy.callMethod("AttachDevice")
        .onInterface(INTERFACE_NAME)
        .withArguments(seat_id, sysfs_path, interactive);
  }

  void FlushDevices(const bool& interactive) {
    m_proxy.callMethod("FlushDevices")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void PowerOff(const bool& interactive) {
    m_proxy.callMethod("PowerOff")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void PowerOffWithFlags(const uint64_t& flags) {
    m_proxy.callMethod("PowerOffWithFlags")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  void Reboot(const bool& interactive) {
    m_proxy.callMethod("Reboot")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void RebootWithFlags(const uint64_t& flags) {
    m_proxy.callMethod("RebootWithFlags")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  void Halt(const bool& interactive) {
    m_proxy.callMethod("Halt")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void HaltWithFlags(const uint64_t& flags) {
    m_proxy.callMethod("HaltWithFlags")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  void Suspend(const bool& interactive) {
    m_proxy.callMethod("Suspend")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void SuspendWithFlags(const uint64_t& flags) {
    m_proxy.callMethod("SuspendWithFlags")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  void Hibernate(const bool& interactive) {
    m_proxy.callMethod("Hibernate")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void HibernateWithFlags(const uint64_t& flags) {
    m_proxy.callMethod("HibernateWithFlags")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  void HybridSleep(const bool& interactive) {
    m_proxy.callMethod("HybridSleep")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void HybridSleepWithFlags(const uint64_t& flags) {
    m_proxy.callMethod("HybridSleepWithFlags")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  void SuspendThenHibernate(const bool& interactive) {
    m_proxy.callMethod("SuspendThenHibernate")
        .onInterface(INTERFACE_NAME)
        .withArguments(interactive);
  }

  void SuspendThenHibernateWithFlags(const uint64_t& flags) {
    m_proxy.callMethod("SuspendThenHibernateWithFlags")
        .onInterface(INTERFACE_NAME)
        .withArguments(flags);
  }

  std::string CanPowerOff() {
    std::string result;
    m_proxy.callMethod("CanPowerOff")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string CanReboot() {
    std::string result;
    m_proxy.callMethod("CanReboot")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string CanHalt() {
    std::string result;
    m_proxy.callMethod("CanHalt")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string CanSuspend() {
    std::string result;
    m_proxy.callMethod("CanSuspend")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string CanHibernate() {
    std::string result;
    m_proxy.callMethod("CanHibernate")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string CanHybridSleep() {
    std::string result;
    m_proxy.callMethod("CanHybridSleep")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  std::string CanSuspendThenHibernate() {
    std::string result;
    m_proxy.callMethod("CanSuspendThenHibernate")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void ScheduleShutdown(const std::string& type, const uint64_t& usec) {
    m_proxy.callMethod("ScheduleShutdown")
        .onInterface(INTERFACE_NAME)
        .withArguments(type, usec);
  }

  bool CancelScheduledShutdown() {
    bool result;
    m_proxy.callMethod("CancelScheduledShutdown")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  sdbus::UnixFd Inhibit(const std::string& what,
                        const std::string& who,
                        const std::string& why,
                        const std::string& mode) {
    sdbus::UnixFd result;
    m_proxy.callMethod("Inhibit")
        .onInterface(INTERFACE_NAME)
        .withArguments(what, who, why, mode)
        .storeResultsTo(result);
    return result;
  }

  std::string CanRebootParameter() {
    std::string result;
    m_proxy.callMethod("CanRebootParameter")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void SetRebootParameter(const std::string& parameter) {
    m_proxy.callMethod("SetRebootParameter")
        .onInterface(INTERFACE_NAME)
        .withArguments(parameter);
  }

  std::string CanRebootToFirmwareSetup() {
    std::string result;
    m_proxy.callMethod("CanRebootToFirmwareSetup")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void SetRebootToFirmwareSetup(const bool& enable) {
    m_proxy.callMethod("SetRebootToFirmwareSetup")
        .onInterface(INTERFACE_NAME)
        .withArguments(enable);
  }

  std::string CanRebootToBootLoaderMenu() {
    std::string result;
    m_proxy.callMethod("CanRebootToBootLoaderMenu")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void SetRebootToBootLoaderMenu(const uint64_t& timeout) {
    m_proxy.callMethod("SetRebootToBootLoaderMenu")
        .onInterface(INTERFACE_NAME)
        .withArguments(timeout);
  }

  std::string CanRebootToBootLoaderEntry() {
    std::string result;
    m_proxy.callMethod("CanRebootToBootLoaderEntry")
        .onInterface(INTERFACE_NAME)
        .storeResultsTo(result);
    return result;
  }

  void SetRebootToBootLoaderEntry(const std::string& boot_loader_entry) {
    m_proxy.callMethod("SetRebootToBootLoaderEntry")
        .onInterface(INTERFACE_NAME)
        .withArguments(boot_loader_entry);
  }

  void SetWallMessage(const std::string& wall_message, const bool& enable) {
    m_proxy.callMethod("SetWallMessage")
        .onInterface(INTERFACE_NAME)
        .withArguments(wall_message, enable);
  }

 public:
  bool EnableWallMessages() {
    return m_proxy.getProperty("EnableWallMessages")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  void EnableWallMessages(const bool& value) {
    m_proxy.setProperty("EnableWallMessages")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  std::string WallMessage() {
    return m_proxy.getProperty("WallMessage")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  void WallMessage(const std::string& value) {
    m_proxy.setProperty("WallMessage")
        .onInterface(INTERFACE_NAME)
        .toValue(value);
  }

  uint32_t NAutoVTs() {
    return m_proxy.getProperty("NAutoVTs")
        .onInterface(INTERFACE_NAME)
        .get<uint32_t>();
  }

  std::vector<std::string> KillOnlyUsers() {
    return m_proxy.getProperty("KillOnlyUsers")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::vector<std::string> KillExcludeUsers() {
    return m_proxy.getProperty("KillExcludeUsers")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  bool KillUserProcesses() {
    return m_proxy.getProperty("KillUserProcesses")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  std::string RebootParameter() {
    return m_proxy.getProperty("RebootParameter")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  bool RebootToFirmwareSetup() {
    return m_proxy.getProperty("RebootToFirmwareSetup")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  uint64_t RebootToBootLoaderMenu() {
    return m_proxy.getProperty("RebootToBootLoaderMenu")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  std::string RebootToBootLoaderEntry() {
    return m_proxy.getProperty("RebootToBootLoaderEntry")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::vector<std::string> BootLoaderEntries() {
    return m_proxy.getProperty("BootLoaderEntries")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  bool IdleHint() {
    return m_proxy.getProperty("IdleHint")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  uint64_t IdleSinceHint() {
    return m_proxy.getProperty("IdleSinceHint")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t IdleSinceHintMonotonic() {
    return m_proxy.getProperty("IdleSinceHintMonotonic")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  std::string BlockInhibited() {
    return m_proxy.getProperty("BlockInhibited")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string DelayInhibited() {
    return m_proxy.getProperty("DelayInhibited")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  uint64_t InhibitDelayMaxUSec() {
    return m_proxy.getProperty("InhibitDelayMaxUSec")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t UserStopDelayUSec() {
    return m_proxy.getProperty("UserStopDelayUSec")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  std::string HandlePowerKey() {
    return m_proxy.getProperty("HandlePowerKey")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandlePowerKeyLongPress() {
    return m_proxy.getProperty("HandlePowerKeyLongPress")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleRebootKey() {
    return m_proxy.getProperty("HandleRebootKey")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleRebootKeyLongPress() {
    return m_proxy.getProperty("HandleRebootKeyLongPress")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleSuspendKey() {
    return m_proxy.getProperty("HandleSuspendKey")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleSuspendKeyLongPress() {
    return m_proxy.getProperty("HandleSuspendKeyLongPress")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleHibernateKey() {
    return m_proxy.getProperty("HandleHibernateKey")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleHibernateKeyLongPress() {
    return m_proxy.getProperty("HandleHibernateKeyLongPress")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleLidSwitch() {
    return m_proxy.getProperty("HandleLidSwitch")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleLidSwitchExternalPower() {
    return m_proxy.getProperty("HandleLidSwitchExternalPower")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string HandleLidSwitchDocked() {
    return m_proxy.getProperty("HandleLidSwitchDocked")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  uint64_t HoldoffTimeoutUSec() {
    return m_proxy.getProperty("HoldoffTimeoutUSec")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  std::string IdleAction() {
    return m_proxy.getProperty("IdleAction")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  uint64_t IdleActionUSec() {
    return m_proxy.getProperty("IdleActionUSec")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  bool PreparingForShutdown() {
    return m_proxy.getProperty("PreparingForShutdown")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool PreparingForSleep() {
    return m_proxy.getProperty("PreparingForSleep")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  sdbus::Struct<std::string, uint64_t> ScheduledShutdown() {
    return m_proxy.getProperty("ScheduledShutdown")
        .onInterface(INTERFACE_NAME)
        .get<sdbus::Struct<std::string, uint64_t>>();
  }

  bool Docked() {
    return m_proxy.getProperty("Docked")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool LidClosed() {
    return m_proxy.getProperty("LidClosed")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool OnExternalPower() {
    return m_proxy.getProperty("OnExternalPower")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  bool RemoveIPC() {
    return m_proxy.getProperty("RemoveIPC")
        .onInterface(INTERFACE_NAME)
        .get<bool>();
  }

  uint64_t RuntimeDirectorySize() {
    return m_proxy.getProperty("RuntimeDirectorySize")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t RuntimeDirectoryInodesMax() {
    return m_proxy.getProperty("RuntimeDirectoryInodesMax")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t InhibitorsMax() {
    return m_proxy.getProperty("InhibitorsMax")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t NCurrentInhibitors() {
    return m_proxy.getProperty("NCurrentInhibitors")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t SessionsMax() {
    return m_proxy.getProperty("SessionsMax")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t NCurrentSessions() {
    return m_proxy.getProperty("NCurrentSessions")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

  uint64_t StopIdleSessionUSec() {
    return m_proxy.getProperty("StopIdleSessionUSec")
        .onInterface(INTERFACE_NAME)
        .get<uint64_t>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace login1
}  // namespace freedesktop
}  // namespace org

#endif
