// Copyright (c) 2025 Joel Winarske
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_GEOCLUE2_GEOCLUE2_LOCATION_H
#define SRC_GEOCLUE2_GEOCLUE2_LOCATION_H

#include "../proxy/org/freedesktop/GeoClue2/Client/Location/location_proxy.h"

#include "../utils/utils.h"

class GeoClue2Location final : public sdbus::ProxyInterfaces<
                                   sdbus::Properties_proxy,
                                   org::freedesktop::GeoClue2::Location_proxy> {
 public:
  struct properties_t {
    double Accuracy;
    double Altitude;
    std::string Description;
    double Heading;
    double Latitude;
    double Longitude;
    double Speed;
    struct {
      uint64_t tv_sec;
      uint64_t tv_nsec;
    } Timestamp;
  };

  explicit GeoClue2Location(sdbus::IConnection& connection,
                            const sdbus::ObjectPath& objectPath)
      : ProxyInterfaces{connection, sdbus::ServiceName(kBusName), objectPath} {
    registerProxy();
    const auto properties = this->GetAll("org.freedesktop.GeoClue2.Location");
    GeoClue2Location::onPropertiesChanged(sdbus::InterfaceName(kBusName),
                                          properties, {});
  }

  virtual ~GeoClue2Location() { unregisterProxy(); }

  [[nodiscard]] const properties_t& Properties() const { return properties_; }

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    for (const auto& [key, value] : changedProperties) {
      if (key == "Accuracy") {
        properties_.Accuracy = value.get<double>();
      } else if (key == "Altitude") {
        properties_.Altitude = value.get<double>();
      } else if (key == "Description") {
        properties_.Description = value.get<std::string>();
      } else if (key == "Heading") {
        properties_.Heading = value.get<double>();
      } else if (key == "Latitude") {
        properties_.Latitude = value.get<double>();
      } else if (key == "Longitude") {
        properties_.Longitude = value.get<double>();
      } else if (key == "Speed") {
        properties_.Speed = value.get<double>();
      } else if (key == "Timestamp") {
        const auto arg_tt = value.get<sdbus::Struct<uint64_t, uint64_t>>();
        properties_.Timestamp.tv_sec = std::get<0>(arg_tt);
        properties_.Timestamp.tv_nsec = std::get<1>(arg_tt);
      } else {
        Utils::print_changed_properties(interfaceName, changedProperties,
                                        invalidatedProperties);
      }
    }
  }

 private:
  properties_t properties_{};
  static constexpr auto kBusName = "org.freedesktop.GeoClue2";
};

#endif  // SRC_GEOCLUE2_GEOCLUE2_LOCATION_H
