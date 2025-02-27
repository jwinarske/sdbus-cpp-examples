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

#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <iomanip>
#include <sstream>

#include <sdbus-c++/sdbus-c++.h>
#include <spdlog/spdlog.h>

struct Utils {
  /// NOLINTNEXTLINE
  static void append_property(const sdbus::Variant& value,
                              std::stringstream& ss) {
    if (const std::string type = value.peekValueType(); type == "u") {
      const auto v = value.get<std::uint32_t>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "d") {
      const auto v = value.get<double>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "n") {
      const auto v = value.get<std::int16_t>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "o") {
      auto object_path = value.get<sdbus::ObjectPath>();
      ss << object_path << std::endl;
    } else if (type == "ao") {
      for (auto object_paths = value.get<std::vector<sdbus::ObjectPath>>();
           const auto& object_path : object_paths) {
        ss << object_path << std::endl;
      }
    } else if (type == "i") {
      const auto v = value.get<std::int32_t>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "b") {
      const auto v = value.get<bool>();
      ss << (v ? "True" : "False") << std::endl;
    } else if (type == "y") {
      const auto v = value.get<std::uint8_t>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "q") {
      const auto v = value.get<std::uint16_t>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "s") {
      const auto v = value.get<std::string>();
      ss << (v.empty() ? "\"\"" : v) << std::endl;
    } else if (type == "x") {
      const auto v = value.get<std::int64_t>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "t") {
      const auto v = value.get<std::uint64_t>();
      ss << std::to_string(v) << std::endl;
    } else if (type == "as") {
      ss << std::endl;
      const auto v = value.get<std::vector<std::string>>();
      for (const auto& s : v) {
        ss << "\t" << s << std::endl;
      }
      if (v.empty())
        ss << "\t" << "\"\"" << std::endl;
    } else if (type == "au") {
      ss << std::endl;
      const auto v = value.get<std::vector<std::uint32_t>>();
      for (const auto& s : v) {
        ss << "\t" << std::to_string(s) << std::endl;
      }
      if (v.empty())
        ss << "\t" << "\"\"" << std::endl;
      else
        ss << std::endl;
    } else if (type == "aau") {
      ss << std::endl;
      const auto v = value.get<std::vector<std::vector<std::uint32_t>>>();
      for (const auto& it : v) {
        for (const auto& address : it) {
          ss << "\t" << std::to_string(address) << std::endl;
        }
      }
      if (v.empty())
        ss << "\t" << "\"\"" << std::endl;
      else
        ss << std::endl;
    } else if (type == "ay") {
      const auto v = value.get<std::vector<uint8_t>>();
      for (const auto& b : v) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(b) << " ";
      }
      if (v.empty())
        ss << "\t" << "\"\"" << std::endl;
      else
        ss << std::endl;
    } else if (type == "aay") {
      const auto v = value.get<std::vector<std::vector<uint8_t>>>();
      for (const auto& it : v) {
        for (const auto& b : it) {
          ss << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(b) << " ";
        }
      }
      if (v.empty())
        ss << "\t" << "\"\"" << std::endl;
      else
        ss << std::endl;
    } else if (type == "a{qv}") {
      ss << std::endl;
      for (auto arg_a_qv = value.get<std::map<std::uint16_t, sdbus::Variant>>();
           const auto& [key, value] : arg_a_qv) {
        ss << "\t" << key << ": ";
        append_property(value, ss);
      }
    } else if (type == "a{sv}") {
      ss << std::endl;
      for (auto arg_a_sv = value.get<std::map<std::string, sdbus::Variant>>();
           const auto& [key, value] : arg_a_sv) {
        ss << "\t" << key << ": ";
        append_property(value, ss);
      }
    } else if (type == "a(sa{sv})") {
      ss << std::endl;
      auto v = value.get<std::vector<
          sdbus::Struct<std::string, std::map<std::string, sdbus::Variant>>>>();
      for (const auto& tuple : v) {
        ss << "\t" << std::get<0>(tuple) << ": ";
        for (const auto& [key, value] : std::get<1>(tuple)) {
          ss << key << ": ";
          append_property(value, ss);
        }
      }
    } else if (type == "(qqy)") {
      ss << std::endl;
      auto v =
          value
              .get<sdbus::Struct<std::uint16_t, std::uint16_t, std::uint8_t>>();
      ss << "\t" << std::get<0>(v) << ": " << std::get<1>(v) << ": "
         << std::get<2>(v);
    } else if (type == "a(qqy)") {
      ss << std::endl;
      auto v = value.get<std::vector<
          sdbus::Struct<std::uint16_t, std::uint16_t, std::uint8_t>>>();
      for (const auto& tuple : v) {
        ss << "\t";
        ss << std::get<0>(tuple) << ": " << std::get<1>(tuple) << ": "
           << std::get<2>(tuple) << std::endl;
      }
    } else if (type == "a(ayuay)") {
      ss << std::endl;
      try {
        auto v = value.get<
            std::vector<sdbus::Struct<std::vector<std::uint8_t>, std::uint32_t,
                                      std::vector<std::uint8_t>>>>();
        for (const auto& tuple : v) {
          const auto& array1 = std::get<0>(tuple);
          const auto& uint_value = std::get<1>(tuple);
          const auto& array2 = std::get<2>(tuple);

          ss << "\t";
          for (const auto& b : array1) {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(b) << " ";
          }
          ss << ", " << std::to_string(uint_value) << ", ";
          for (const auto& b : array2) {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(b) << " ";
          }
          ss << std::endl;
        }
      } catch (const sdbus::Error& e) {
        ss << "Error: " << e.what() << std::endl;
        ss << "Type of value: " << value.peekValueType() << std::endl;
      }
    } else if (type == "a(ayuayu)") {
      ss << std::endl;
      try {
        auto v = value.get<std::vector<
            sdbus::Struct<std::vector<std::uint8_t>, std::uint32_t,
                          std::vector<std::uint8_t>, std::uint32_t>>>();
        for (const auto& tuple : v) {
          const auto& array1 = std::get<0>(tuple);
          const auto& uint_value1 = std::get<1>(tuple);
          const auto& array2 = std::get<2>(tuple);
          const auto& uint_value2 = std::get<3>(tuple);

          ss << "\t";
          for (const auto& b : array1) {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(b) << " ";
          }
          ss << ", " << std::to_string(uint_value1) << ", ";
          for (const auto& b : array2) {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(b) << " ";
          }
          ss << ", " << std::to_string(uint_value2) << std::endl;
        }
      } catch (const sdbus::Error& e) {
        ss << "Error: " << e.what() << std::endl;
        ss << "Type of value: " << type << std::endl;
      }
    } else if (type == "aa{sv}") {
      ss << std::endl;
      for (auto arg_aa_sv =
               value.get<std::vector<std::map<std::string, sdbus::Variant>>>();
           const auto& arg_a_sv : arg_aa_sv) {
        for (const auto& [key, value] : arg_a_sv) {
          ss << "\t" << key << ": ";
          append_property(value, ss);
        }
      }
    } else if (type == "(tt)") {
      ss << std::endl;
      auto arg_tt = value.get<sdbus::Struct<uint64_t, uint64_t>>();
      ss << "\t" << std::get<0>(arg_tt) << ": " << std::get<1>(arg_tt);
    } else if (type == "(ttt)") {
      ss << std::endl;
      auto arg_ttt = value.get<sdbus::Struct<uint64_t, uint64_t, uint64_t>>();
      ss << "\t" << std::get<0>(arg_ttt) << ": " << std::get<1>(arg_ttt) << ": "
         << std::get<2>(arg_ttt);
    } else if (type == "(tttt)") {
      ss << std::endl;
      auto arg_tttt =
          value.get<sdbus::Struct<uint64_t, uint64_t, uint64_t, uint64_t>>();
      ss << "\t" << std::get<0>(arg_tttt) << ": " << std::get<1>(arg_tttt)
         << ": " << std::get<2>(arg_tttt) << ": " << std::get<3>(arg_tttt);
    } else if (type == "(so)") {
      ss << std::endl;
      auto arg_so = value.get<sdbus::Struct<std::string, sdbus::ObjectPath>>();
      ss << "\t" << std::get<0>(arg_so) << ": " << std::get<1>(arg_so);
    } else if (type == "(st)") {
      ss << std::endl;
      auto arg_so = value.get<sdbus::Struct<std::string, std::uint64_t>>();
      ss << "\t" << std::get<0>(arg_so) << ": " << std::get<1>(arg_so);
    } else if (type == "(uo)") {
      ss << std::endl;
      auto arg_uo =
          value.get<sdbus::Struct<std::uint32_t, sdbus::ObjectPath>>();
      ss << "\t" << std::get<0>(arg_uo) << ": " << std::get<1>(arg_uo);
    } else if (type == "a(isb)") {
      ss << std::endl;
      for (auto args = value.get<std::vector<
                           sdbus::Struct<std::int32_t, std::string, bool>>>();
           const auto& arg_a_isb : args) {
        ss << "\t" << std::get<0>(arg_a_isb) << ": " << std::get<1>(arg_a_isb)
           << ": " << (std::get<2>(arg_a_isb) ? "True" : "False");
      }
    } else if (type == "(iiay)") {
      ss << std::endl;
      auto args = value.get<sdbus::Struct<std::int32_t, std::int32_t,
                                          std::vector<std::uint8_t>>>();
      ss << "\t" << std::get<0>(args) << ": " << std::get<1>(args) << ": ";
      for (const auto& b : std::get<2>(args)) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(b) << " ";
      }
    } else if (type == "a(iiay)") {
      ss << std::endl;
      for (auto args =
               value.get<std::vector<sdbus::Struct<
                   std::int32_t, std::int32_t, std::vector<std::uint8_t>>>>();
           const auto& arg_a_iiay : args) {
        ss << "\t" << std::get<0>(arg_a_iiay) << ": " << std::get<1>(arg_a_iiay)
           << ": ";
        for (const auto& b : std::get<2>(arg_a_iiay)) {
          ss << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(b) << " ";
        }
      }
    } else if (type == "(iiayqs)") {
      ss << std::endl;
      auto arg_iiayqs = value.get<
          sdbus::Struct<std::int32_t, std::int32_t, std::vector<std::uint8_t>,
                        std::uint16_t, std::string>>();
      ss << "\t" << std::get<0>(arg_iiayqs) << ", " << std::get<1>(arg_iiayqs)
         << ", ";
      for (const auto& b : std::get<2>(arg_iiayqs)) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(b) << " ";
      }
      ss << ", " << std::get<3>(arg_iiayqs) << ", " << std::get<4>(arg_iiayqs);
    } else if (type == "a(iiayqs)") {
      ss << std::endl;
      for (auto args =
               value.get<std::vector<sdbus::Struct<
                   std::int32_t, std::int32_t, std::vector<std::uint8_t>,
                   std::uint16_t, std::string>>>();
           const auto& arg_a_iiayqs : args) {
        ss << "\t" << std::get<0>(arg_a_iiayqs) << ", "
           << std::get<1>(arg_a_iiayqs) << ", ";
        for (const auto& b : std::get<2>(arg_a_iiayqs)) {
          ss << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(b) << " ";
        }
        ss << ", " << std::get<3>(arg_a_iiayqs) << ", "
           << std::get<4>(arg_a_iiayqs);
      }
    } else if (type == "a(so)") {
      ss << std::endl;
      for (auto args = value.get<std::vector<
                           sdbus::Struct<std::string, sdbus::ObjectPath>>>();
           const auto& arg_so : args) {
        ss << "\t" << std::get<0>(arg_so) << ": " << std::get<1>(arg_so);
      }
    } else {
      ss << "Unknown type: " << type << std::endl;
    }
  }

  static void append_properties(
      const std::map<sdbus::MemberName, sdbus::Variant>& props,
      std::stringstream& ss) {
    for (auto& [key, value] : props) {
      ss << "  " << key << ": ";
      append_property(value, ss);
    }
  }

  static void append_properties(
      const std::map<std::string, sdbus::Variant>& props,
      std::stringstream& ss) {
    for (auto& [key, value] : props) {
      ss << "  " << key << ": ";
      append_property(value, ss);
    }
  }

  static void print_changed_properties(
      const sdbus::InterfaceName& interfaceName,
      const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
      const std::vector<sdbus::PropertyName>& invalidatedProperties) {
    std::stringstream ss;
    ss << std::endl;
    ss << "[" << interfaceName << "] Properties changed" << std::endl;
    append_properties(changedProperties, ss);
    for (const auto& name : invalidatedProperties) {
      ss << "[" << interfaceName << "] Invalidated property: " << name
         << std::endl;
    }
    spdlog::info("{}", ss.str());
  }

  static std::vector<std::string> ListNames(sdbus::IConnection& connection) {
    const auto proxy = sdbus::createProxy(
        connection, sdbus::ServiceName("org.freedesktop.DBus"),
        sdbus::ObjectPath("/org/freedesktop/DBus"));

    std::vector<std::string> dbus_interfaces;
    proxy->callMethod("ListNames")
        .onInterface("org.freedesktop.DBus")
        .storeResultsTo(dbus_interfaces);

    return std::move(dbus_interfaces);
  }

  static bool isServicePresent(const std::vector<std::string>& dbus_interfaces,
                               const std::string_view& service) {
    return std::find(dbus_interfaces.begin(), dbus_interfaces.end(), service) !=
           dbus_interfaces.end();
  }
};

#endif  // SRC_UTILS_H
