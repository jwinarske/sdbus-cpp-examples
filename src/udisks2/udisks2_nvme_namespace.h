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

#ifndef SRC_UDISK2_UDISK2_NVME_NAMESPACE_H
#define SRC_UDISK2_UDISK2_NVME_NAMESPACE_H

#include "../proxy/UDisks2NvmeNamespace_proxy.h"
#include "../utils.h"

class UDisks2NvmeNamespace final
    : public sdbus::ProxyInterfaces<
          sdbus::Properties_proxy,
          org::freedesktop::UDisks2::NVMe::Namespace_proxy> {
 public:
  UDisks2NvmeNamespace(sdbus::IConnection& connection,
                       const sdbus::ObjectPath& objectPath)
      : ProxyInterfaces{connection, sdbus::ServiceName(kBusName), objectPath},
        connection_(connection),
        object_path_(objectPath) {
    registerProxy();
    const auto properties =
        this->GetAll("org.freedesktop.UDisks2.NVMe.Namespace");
    UDisks2NvmeNamespace::onPropertiesChanged(
        sdbus::InterfaceName("org.freedesktop.UDisks2.NVMe.Namespace"),
        properties, {});
  }

  ~UDisks2NvmeNamespace() { unregisterProxy(); }

 private:
  static constexpr char kBusName[] = "org.freedesktop.UDisks2";

  sdbus::IConnection& connection_;
  sdbus::ObjectPath object_path_;

  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override {
    std::stringstream ss;
    ss << std::endl;
    ss << "[" << interfaceName << "] UDisks2Block Properties changed"
       << std::endl;
    Utils::append_properties(changedProperties, ss);
    for (const auto& name : invalidatedProperties) {
      ss << "[" << interfaceName << "] Invalidated property: " << name
         << std::endl;
    }
    spdlog::info("{}", ss.str());
  }
};

#endif  // SRC_UDISK2_UDISK2_NVME_NAMESPACE_H
