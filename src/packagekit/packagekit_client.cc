
#include "packagekit_client.h"

#include "../utils/utils.h"

PackageKitClient::PackageKitClient(sdbus::IConnection& connection)
    : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                      sdbus::ObjectPath(OBJECT_PATH)},
      object_path_(sdbus::ObjectPath(OBJECT_PATH)) {
  registerProxy();
}

PackageKitClient::~PackageKitClient() {
  unregisterProxy();
}

void PackageKitClient::onTransactionListChanged(
    const std::vector<std::string>& transactions) {
  std::ostringstream os;
  os << std::endl;
  os << "[PackageKitClient] onTransactionListChanged (" << transactions.size()
     << ")" << std::endl;
  for (const auto& transaction : transactions) {
    os << transaction << std::endl;
  }
  spdlog::info(os.str());
}

void PackageKitClient::onRestartSchedule() {
  spdlog::info("onRestartSchedule");
}

void PackageKitClient::onRepoListChanged() {
  spdlog::info("onRepoListChanged");
}

void PackageKitClient::onUpdatesChanged() {
  spdlog::info("onUpdatesChanged");
}

void PackageKitClient::onPropertiesChanged(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  Utils::print_changed_properties(interfaceName, changedProperties,
                                  invalidatedProperties);
}
