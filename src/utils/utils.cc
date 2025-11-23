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

#include "utils.h"

#include <cmath>
#include <iomanip>

void Utils::append_property(const sdbus::Variant& value,
                            std::ostringstream& os) {
  const std::string_view type = value.peekValueType();
  os << "[" << type << "] ";
  if (const auto it = type_map_.find(type); it != type_map_.end()) {
    it->second(value, os);
  } else {
    os << "Unknown type: " << type << std::endl;
  }
}

void Utils::append_properties(
    const std::map<sdbus::MemberName, sdbus::Variant>& props,
    std::ostringstream& os) {
  for (auto& [key, value] : props) {
    os << "  " << key << ": ";
    append_property(value, os);
  }
}

void Utils::append_properties(
    const std::map<std::string, sdbus::Variant>& props,
    std::ostringstream& os) {
  for (auto& [key, value] : props) {
    os << "  " << key << ": ";
    append_property(value, os);
  }
}

void Utils::print_changed_properties(
    const sdbus::InterfaceName& interfaceName,
    const std::map<sdbus::PropertyName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::PropertyName>& invalidatedProperties) {
  std::ostringstream os;
  os << std::endl;
  os << "[" << interfaceName << "] Properties changed" << std::endl;
  append_properties(changedProperties, os);
  for (const auto& name : invalidatedProperties) {
    os << "[" << interfaceName << "] Invalidated property: " << name
       << std::endl;
  }
  spdlog::info(os.str());
}

std::vector<std::string> Utils::ListNames(sdbus::IConnection& connection) {
  const auto proxy =
      createProxy(connection, sdbus::ServiceName("org.freedesktop.DBus"),
                  sdbus::ObjectPath("/org/freedesktop/DBus"));

  std::vector<std::string> dbus_interfaces;
  proxy->callMethod("ListNames")
      .onInterface("org.freedesktop.DBus")
      .storeResultsTo(dbus_interfaces);

  return dbus_interfaces;
}

bool Utils::isServicePresent(const std::vector<std::string>& dbus_interfaces,
                             const std::string_view& service) {
  return std::ranges::find(dbus_interfaces, service) != dbus_interfaces.end();
}

const std::unordered_map<
    std::string_view,
    std::function<void(const sdbus::Variant&, std::ostringstream&)>>
    Utils::
        type_map_ = {{"u",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<std::uint32_t>())
                           << std::endl;
                      }},
                     {"d",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<double>()) << std::endl;
                      }},
                     {"n",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<std::int16_t>())
                           << std::endl;
                      }},
                     {"o",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << v.get<sdbus::ObjectPath>() << std::endl;
                      }},
                     {"ao",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        for (const auto& object_path :
                             v.get<std::vector<sdbus::ObjectPath>>()) {
                          os << object_path << std::endl;
                        }
                      }},
                     {"i",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<std::int32_t>())
                           << std::endl;
                      }},
                     {"b",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << (v.get<bool>() ? "True" : "False") << std::endl;
                      }},
                     {"y",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<std::uint8_t>())
                           << std::endl;
                      }},
                     {"q",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<std::uint16_t>())
                           << std::endl;
                      }},
                     {"s",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << (v.get<std::string>().empty()
                                   ? "\"\""
                                   : v.get<std::string>())
                           << std::endl;
                      }},
                     {"x",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<std::int64_t>())
                           << std::endl;
                      }},
                     {"t",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::to_string(v.get<std::uint64_t>())
                           << std::endl;
                      }},
                     {"as",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& s :
                             v.get<std::vector<std::string>>()) {
                          os << "\t" << s << std::endl;
                        }
                        if (v.get<std::vector<std::string>>().empty())
                          os << "\t" << "\"\"" << std::endl;
                      }},
                     {"au",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& s :
                             v.get<std::vector<std::uint32_t>>()) {
                          os << "\t" << std::to_string(s) << std::endl;
                        }
                        if (v.get<std::vector<std::uint32_t>>().empty())
                          os << "\t" << "\"\"" << std::endl;
                        else
                          os << std::endl;
                      }},
                     {"aau",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& it :
                             v.get<std::vector<std::vector<std::uint32_t>>>()) {
                          for (const auto& address : it) {
                            os << "\t" << std::to_string(address) << std::endl;
                          }
                        }
                        if (v.get<std::vector<std::vector<std::uint32_t>>>()
                                .empty())
                          os << "\t" << "\"\"" << std::endl;
                        else
                          os << std::endl;
                      }},
                     {"ay",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        for (const auto& b : v.get<std::vector<uint8_t>>()) {
                          os << std::hex << std::setw(2) << std::setfill('0')
                             << static_cast<int>(b) << " ";
                        }
                        if (v.get<std::vector<uint8_t>>().empty())
                          os << "\t" << "\"\"" << std::endl;
                        else
                          os << std::endl;
                      }},
                     {"aay",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        for (const auto& it :
                             v.get<std::vector<std::vector<uint8_t>>>()) {
                          for (const auto& b : it) {
                            os << std::hex << std::setw(2) << std::setfill('0')
                               << static_cast<int>(b) << " ";
                          }
                        }
                        if (v.get<std::vector<std::vector<uint8_t>>>().empty())
                          os << "\t" << "\"\"" << std::endl;
                        else
                          os << std::endl;
                      }},
                     {"a{qv}",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& [key, value] :
                             v.get<std::map<std::uint16_t, sdbus::Variant>>()) {
                          os << "\t" << key << ": ";
                          append_property(value, os);
                        }
                      }},
                     {"a{sv}",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& [key, value] :
                             v.get<std::map<std::string, sdbus::Variant>>()) {
                          os << "\t" << key << ": ";
                          append_property(value, os);
                        }
                      }},
                     {"a(sa{sv})",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& tuple :
                             v.get<std::vector<sdbus::Struct<
                                 std::string,
                                 std::map<std::string, sdbus::Variant>>>>()) {
                          os << "\t" << std::get<0>(tuple) << ": ";
                          for (const auto& [key, value] : std::get<1>(tuple)) {
                            os << key << ": ";
                            append_property(value, os);
                          }
                        }
                      }},
                     {"(qqy)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        const auto tuple =
                            v.get<sdbus::Struct<std::uint16_t, std::uint16_t,
                                                std::uint8_t>>();
                        os << "\t" << std::get<0>(tuple) << ": "
                           << std::get<1>(tuple) << ": " << std::get<2>(tuple);
                      }},
                     {"a(qqy)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& tuple :
                             v.get<std::vector<
                                 sdbus::Struct<std::uint16_t, std::uint16_t,
                                               std::uint8_t>>>()) {
                          os << "\t" << std::get<0>(tuple) << ": "
                             << std::get<1>(tuple) << ": " << std::get<2>(tuple)
                             << std::endl;
                        }
                      }},
                     {"a(ayuay)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        try {
                          for (const auto& tuple :
                               v.get<std::vector<sdbus::Struct<
                                   std::vector<std::uint8_t>, std::uint32_t,
                                   std::vector<std::uint8_t>>>>()) {
                            os << "\t";
                            for (const auto& b : std::get<0>(tuple)) {
                              os << std::hex << std::setw(2)
                                 << std::setfill('0') << static_cast<int>(b)
                                 << " ";
                            }
                            os << ", " << std::to_string(std::get<1>(tuple))
                               << ", ";
                            for (const auto& b : std::get<2>(tuple)) {
                              os << std::hex << std::setw(2)
                                 << std::setfill('0') << static_cast<int>(b)
                                 << " ";
                            }
                            os << std::endl;
                          }
                        } catch (const sdbus::Error& e) {
                          os << "Error: " << e.what() << std::endl;
                          os << "Type of value: " << v.peekValueType()
                             << std::endl;
                        }
                      }},
                     {"a(ayuayu)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        try {
                          for (const auto& tuple :
                               v.get<std::vector<sdbus::Struct<
                                   std::vector<std::uint8_t>, std::uint32_t,
                                   std::vector<std::uint8_t>,
                                   std::uint32_t>>>()) {
                            os << "\t";
                            for (const auto& b : std::get<0>(tuple)) {
                              os << std::hex << std::setw(2)
                                 << std::setfill('0') << static_cast<int>(b)
                                 << " ";
                            }
                            os << ", " << std::to_string(std::get<1>(tuple))
                               << ", ";
                            for (const auto& b : std::get<2>(tuple)) {
                              os << std::hex << std::setw(2)
                                 << std::setfill('0') << static_cast<int>(b)
                                 << " ";
                            }
                            os << ", " << std::to_string(std::get<3>(tuple))
                               << std::endl;
                          }
                        } catch (const sdbus::Error& e) {
                          os << "Error: " << e.what() << std::endl;
                          os << "Type of value: " << v.peekValueType()
                             << std::endl;
                        }
                      }},
                     {"aa{sv}",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& arg_a_sv :
                             v.get<std::vector<
                                 std::map<std::string, sdbus::Variant>>>()) {
                          for (const auto& [key, value] : arg_a_sv) {
                            os << "\t" << key << ": ";
                            append_property(value, os);
                          }
                        }
                      }},
                     {"(tt)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        const auto arg_tt =
                            v.get<sdbus::Struct<uint64_t, uint64_t>>();
                        os << "\t" << std::get<0>(arg_tt) << ": "
                           << std::get<1>(arg_tt);
                      }},
                     {"(ttt)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        const auto arg_ttt = v.get<
                            sdbus::Struct<uint64_t, uint64_t, uint64_t>>();
                        os << "\t" << std::get<0>(arg_ttt) << ": "
                           << std::get<1>(arg_ttt) << ": "
                           << std::get<2>(arg_ttt);
                      }},
                     {"(tttt)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        const auto arg_tttt =
                            v.get<sdbus::Struct<uint64_t, uint64_t, uint64_t,
                                                uint64_t>>();
                        os << "\t" << std::get<0>(arg_tttt) << ": "
                           << std::get<1>(arg_tttt) << ": "
                           << std::get<2>(arg_tttt) << ": "
                           << std::get<3>(arg_tttt);
                      }},
                     {"(so)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        const auto arg_so = v.get<
                            sdbus::Struct<std::string, sdbus::ObjectPath>>();
                        os << "\t" << std::get<0>(arg_so) << ": "
                           << std::get<1>(arg_so);
                      }},
                     {"(st)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        const auto arg_st =
                            v.get<sdbus::Struct<std::string, std::uint64_t>>();
                        os << "\t" << std::get<0>(arg_st) << ": "
                           << std::get<1>(arg_st);
                      }},
                     {"(uo)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        const auto arg_uo = v.get<
                            sdbus::Struct<std::uint32_t, sdbus::ObjectPath>>();
                        os << "\t" << std::get<0>(arg_uo) << ": "
                           << std::get<1>(arg_uo);
                      }},
                     {"a(isb)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& arg_a_isb :
                             v.get<std::vector<sdbus::Struct<
                                 std::int32_t, std::string, bool>>>()) {
                          os << "\t" << std::get<0>(arg_a_isb) << ": "
                             << std::get<1>(arg_a_isb) << ": "
                             << (std::get<2>(arg_a_isb) ? "True" : "False");
                        }
                      }},
                     {"(iiay)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        auto args =
                            v.get<sdbus::Struct<std::int32_t, std::int32_t,
                                                std::vector<std::uint8_t>>>();
                        os << "\t" << std::get<0>(args) << ": "
                           << std::get<1>(args) << ": ";
                        for (const auto& b : std::get<2>(args)) {
                          os << std::hex << std::setw(2) << std::setfill('0')
                             << static_cast<int>(b) << " ";
                        }
                      }},
                     {"a(iiay)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& arg_a_iiay :
                             v.get<std::vector<
                                 sdbus::Struct<std::int32_t, std::int32_t,
                                               std::vector<std::uint8_t>>>>()) {
                          os << "\t" << std::get<0>(arg_a_iiay) << ": "
                             << std::get<1>(arg_a_iiay) << ": ";
                          for (const auto& b : std::get<2>(arg_a_iiay)) {
                            os << std::hex << std::setw(2) << std::setfill('0')
                               << static_cast<int>(b) << " ";
                          }
                        }
                      }},
                     {"(iiayqs)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        auto arg_iiayqs =
                            v.get<sdbus::Struct<std::int32_t, std::int32_t,
                                                std::vector<std::uint8_t>,
                                                std::uint16_t, std::string>>();
                        os << "\t" << std::get<0>(arg_iiayqs) << ", "
                           << std::get<1>(arg_iiayqs) << ", ";
                        for (const auto& b : std::get<2>(arg_iiayqs)) {
                          os << std::hex << std::setw(2) << std::setfill('0')
                             << static_cast<int>(b) << " ";
                        }
                        os << ", " << std::get<3>(arg_iiayqs) << ", "
                           << std::get<4>(arg_iiayqs);
                      }},
                     {"a(iiayqs)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& arg_a_iiayqs :
                             v.get<std::vector<sdbus::Struct<
                                 std::int32_t, std::int32_t,
                                 std::vector<std::uint8_t>, std::uint16_t,
                                 std::string>>>()) {
                          os << "\t" << std::get<0>(arg_a_iiayqs) << ", "
                             << std::get<1>(arg_a_iiayqs) << ", ";
                          for (const auto& b : std::get<2>(arg_a_iiayqs)) {
                            os << std::hex << std::setw(2) << std::setfill('0')
                               << static_cast<int>(b) << " ";
                          }
                          os << ", " << std::get<3>(arg_a_iiayqs) << ", "
                             << std::get<4>(arg_a_iiayqs);
                        }
                      }},
                     {"a(so)",
                      [](const sdbus::Variant& v, std::ostringstream& os) {
                        os << std::endl;
                        for (const auto& arg_so :
                             v.get<std::vector<sdbus::Struct<
                                 std::string, sdbus::ObjectPath>>>()) {
                          os << "\t" << std::get<0>(arg_so) << ": "
                             << std::get<1>(arg_so);
                        }
                      }}};

std::string Utils::scalarToString(const glz::generic& val) {
  if (val.is_null())
    return "\"\"";
  if (val.is_string())
    return val.get<std::string>();
  if (val.is_number()) {
    double d = val.get<double>();
    // Check if the number is an integer to preserve precision
    // Only convert to int64 if it's within a safe range
    constexpr double kMaxSafeInt64 = 9007199254740992.0;   // 2^53
    constexpr double kMinSafeInt64 = -9007199254740992.0;  // -2^53
    if (d >= kMinSafeInt64 && d <= kMaxSafeInt64 && d == std::floor(d)) {
      return std::to_string(static_cast<int64_t>(d));
    }
    return std::to_string(d);
  }
  if (val.is_boolean())
    return val.get<bool>() ? "true" : "false";
  return "[complex]";
}

// NOLINTNEXTLINE(clang-tidy)
std::string Utils::elementToLines(const glz::generic& el, const int indent) {
  std::string out;
  const std::string pad(indent * 2, ' ');

  if (el.is_object()) {
    const auto& obj = el.get<glz::generic::object_t>();
    for (const auto& [key, val] : obj) {
      if (val.is_object() || val.is_array()) {
        out += pad + key + ":\n";
        out += elementToLines(val, indent + 1);
      } else {
        out += pad + key + ": " + scalarToString(val) + "\n";
      }
    }
    return out;
  }

  if (el.is_array()) {
    const auto& arr = el.get<glz::generic::array_t>();
    size_t idx = 0;
    for (const auto& item : arr) {
      std::string idxKey = "[" + std::to_string(idx++) + "]";
      if (item.is_object() || item.is_array()) {
        out += pad + idxKey + ":\n";
        out += elementToLines(item, indent + 1);
      } else {
        out += pad + idxKey + ": " + scalarToString(item) + "\n";
      }
    }
    return out;
  }

  // scalar
  out += pad + scalarToString(el) + "\n";
  return out;
}

std::string Utils::parseDescriptionJson(const std::string& json) {
  if (json.empty()) {
    return "<empty>";
  }

  glz::generic doc;
  auto ec = glz::read_json(doc, json);
  if (ec) {
    return std::string("json_error: ") + glz::format_error(ec, json);
  }

  return elementToLines(doc, 0);
}
