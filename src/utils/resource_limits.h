// Copyright (c) 2026 Joel Winarske
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

#ifndef SRC_UTILS_RESOURCE_LIMITS_H_
#define SRC_UTILS_RESOURCE_LIMITS_H_

#include <cstddef>

namespace resource_limits {

inline constexpr std::size_t kMaxAdapters = 64;
inline constexpr std::size_t kMaxDevices = 1024;
inline constexpr std::size_t kMaxGattServices = 4096;
inline constexpr std::size_t kMaxGattCharacteristics = 16384;
inline constexpr std::size_t kMaxGattDescriptors = 32768;
inline constexpr std::size_t kMaxBatteryEntries = 1024;
inline constexpr std::size_t kMaxInputEntries = 1024;
inline constexpr std::size_t kMaxUPowerClients = 256;
inline constexpr std::size_t kMaxUnits = 16384;

inline bool IsAtCapacity(const std::size_t current_size,
                         const std::size_t limit) {
  return current_size >= limit;
}

}  // namespace resource_limits

#endif  // SRC_UTILS_RESOURCE_LIMITS_H_
