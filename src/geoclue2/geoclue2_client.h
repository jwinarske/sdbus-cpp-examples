#ifndef SRC_GEOCLUE2_GEOCLUE2_CLIENT_H
#define SRC_GEOCLUE2_GEOCLUE2_CLIENT_H

#include "../proxy/org/freedesktop/GeoClue2/Client/client_proxy.h"
#include "geoclue2_location.h"

#include "../utils/utils.h"

class GeoClue2Client final
    : public sdbus::ProxyInterfaces<sdbus::Properties_proxy,
                                    org::freedesktop::GeoClue2::Client_proxy> {
 public:
  struct properties_t {
    bool Active;
    std::string DesktopId;
    std::uint32_t DistanceThreshold;
    sdbus::ObjectPath Location;
    std::uint32_t RequestedAccuracyLevel;
    std::uint32_t TimeThreshold;
  };

  using LocationPropertiesChangedCallback =
      std::function<void(const GeoClue2Location&)>;

  explicit GeoClue2Client(sdbus::IConnection& connection,
                          const sdbus::ObjectPath& objectPath,
                          LocationPropertiesChangedCallback callback = nullptr)
      : ProxyInterfaces{connection, sdbus::ServiceName(INTERFACE_NAME),
                        objectPath},
        location_callback_(std::move(callback)) {
    registerProxy();
    const auto properties = this->GetAll("org.freedesktop.GeoClue2.Client");
    GeoClue2Client::onPropertiesChanged(sdbus::InterfaceName(INTERFACE_NAME),
                                        properties, {});
  }

  virtual ~GeoClue2Client() { unregisterProxy(); }

  [[nodiscard]] std::shared_ptr<GeoClue2Location> Location() const {
    return location_;
  }

  [[nodiscard]] properties_t Properties() const { return properties_; }

 private:
  properties_t properties_{};
  LocationPropertiesChangedCallback location_callback_;

  static constexpr auto INTERFACE_NAME = "org.freedesktop.GeoClue2";

  std::shared_ptr<GeoClue2Location> location_;

  void onLocationUpdated(const sdbus::ObjectPath& old,
                         const sdbus::ObjectPath& new_) override {
    location_ =
        std::make_shared<GeoClue2Location>(getProxy().getConnection(), new_);

    // Set up property change notification
    if (location_ && location_callback_) {
      // Initial callback with current properties
      location_callback_(*location_);

      // Connect to location's property changes
      location_->getProxy()
          .uponSignal("PropertiesChanged")
          .onInterface(INTERFACE_NAME)
          .call(
              [this](
                  const sdbus::InterfaceName& interface,
                  const std::map<sdbus::PropertyName, sdbus::Variant>& changed,
                  const std::vector<sdbus::PropertyName>& invalidated) {
                if (location_callback_) {
                  location_callback_(*location_);
                }
              });
    }
  }

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    for (const auto& [key, value] : changedProperties) {
      if (key == "Active") {
        properties_.Active = value.get<bool>();
      } else if (key == "DesktopId") {
        properties_.DesktopId = value.get<std::string>();
      } else if (key == "DistanceThreshold") {
        properties_.DistanceThreshold = value.get<std::uint32_t>();
      } else if (key == "Location") {
        properties_.Location = value.get<sdbus::ObjectPath>();
      } else if (key == "RequestedAccuracyLevel") {
        properties_.RequestedAccuracyLevel = value.get<std::uint32_t>();
      } else if (key == "TimeThreshold") {
        properties_.TimeThreshold = value.get<std::uint32_t>();
      } else {
        Utils::print_changed_properties(interfaceName, changedProperties,
                                        invalidatedProperties);
      }
    }
  }
};

#endif  // SRC_GEOCLUE2_GEOCLUE2_CLIENT_H