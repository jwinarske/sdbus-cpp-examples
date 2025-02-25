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

#ifndef SRC_PACKAGEKIT_PACKAGEKIT_TRANSACTION_H
#define SRC_PACKAGEKIT_PACKAGEKIT_TRANSACTION_H

#include "../proxy/org/freedesktop/PackageKit/Transaction/transaction_proxy.h"

class PackageKitTransaction final
    : public sdbus::ProxyInterfaces<
          sdbus::Properties_proxy,
          org::freedesktop::PackageKit::Transaction_proxy> {
 public:
  static constexpr auto INTERFACE_NAME =
      "org.freedesktop.PackageKit.Transaction";

  explicit PackageKitTransaction(sdbus::IConnection& connection,
                                 const sdbus::ObjectPath& objectPath);

  virtual ~PackageKitTransaction();

 private:
  void onPropertiesChanged(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) override;

  void onCategory(const std::string& parent_id,
                  const std::string& cat_id,
                  const std::string& name,
                  const std::string& summary,
                  const std::string& icon) override;
  void onDetails(const std::map<std::string, sdbus::Variant>& data) override;
  void onErrorCode(const uint32_t& code, const std::string& details) override;
  void onFiles(const std::string& package_id,
               const std::vector<std::string>& file_list) override;
  void onFinished(const uint32_t& exit, const uint32_t& runtime) override;
  void onPackage(const uint32_t& info,
                 const std::string& package_id,
                 const std::string& summary) override;
  void onPackages(
      const std::vector<sdbus::Struct<uint32_t, std::string, std::string>>&
          packages) override;
  void onRepoDetail(const std::string& repo_id,
                    const std::string& description,
                    const bool& enabled) override;
  void onRepoSignatureRequired(const std::string& package_id,
                               const std::string& repository_name,
                               const std::string& key_url,
                               const std::string& key_userid,
                               const std::string& key_id,
                               const std::string& key_fingerprint,
                               const std::string& key_timestamp,
                               const uint32_t& type) override;
  void onEulaRequired(const std::string& eula_id,
                      const std::string& package_id,
                      const std::string& vendor_name,
                      const std::string& license_agreement) override;
  void onMediaChangeRequired(const uint32_t& media_type,
                             const std::string& media_id,
                             const std::string& media_text) override;
  void onRequireRestart(const uint32_t& type,
                        const std::string& package_id) override;
  void onTransaction(const sdbus::ObjectPath& object_path,
                     const std::string& timespec,
                     const bool& succeeded,
                     const uint32_t& role,
                     const uint32_t& duration,
                     const std::string& data,
                     const uint32_t& uid,
                     const std::string& cmdline) override;
  void onUpdateDetail(const std::string& package_id,
                      const std::vector<std::string>& updates,
                      const std::vector<std::string>& obsoletes,
                      const std::vector<std::string>& vendor_urls,
                      const std::vector<std::string>& bugzilla_urls,
                      const std::vector<std::string>& cve_urls,
                      const uint32_t& restart,
                      const std::string& update_text,
                      const std::string& changelog,
                      const uint32_t& state,
                      const std::string& issued,
                      const std::string& updated) override;
  void onUpdateDetails(
      const std::vector<sdbus::Struct<std::string,
                                      std::vector<std::string>,
                                      std::vector<std::string>,
                                      std::vector<std::string>,
                                      std::vector<std::string>,
                                      std::vector<std::string>,
                                      uint32_t,
                                      std::string,
                                      std::string,
                                      uint32_t,
                                      std::string,
                                      std::string>>& details) override;
  void onDistroUpgrade(const uint32_t& type,
                       const std::string& name,
                       const std::string& summary) override;
  void onItemProgress(const std::string& id,
                      const uint32_t& status,
                      const uint32_t& percentage) override;
  void onDestroy() override;
};

#endif  // SRC_PACKAGEKIT_PACKAGEKIT_TRANSACTION_H
