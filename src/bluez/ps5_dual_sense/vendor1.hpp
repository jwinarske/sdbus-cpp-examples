#ifndef HID_PAGE_VENDOR1_HPP
#define HID_PAGE_VENDOR1_HPP

#include "hid/usage.hpp"

namespace hid::page {
enum class vendor1 : std::uint16_t;

template <>
struct info<vendor1> {
  constexpr static page_id_t page_id = 0xFF80;
  constexpr static usage_id_t max_usage_id = 12;
  constexpr static auto name = "Vendor1 Defined";
};

enum class vendor1 : std::uint16_t {

  UNKNOWN_1 = 0x33,
  UNKNOWN_2 = 0x34,
  UNKNOWN_3 = 0x24,
  UNKNOWN_4 = 0x26,
  UNKNOWN_5 = 0x40,
  UNKNOWN_6 = 0x28,
  UNKNOWN_7 = 0x29,
  UNKNOWN_8 = 0x2A,
  UNKNOWN_9 = 0x2B,
  UNKNOWN_10 = 0x31,
  UNKNOWN_11 = 0x32,
  UNKNOWN_12 = 0x30,

  UNKNOWN_13 = 0x2C,
  UNKNOWN_14 = 0x2D,
  UNKNOWN_15 = 0x2E,
  UNKNOWN_16 = 0x2F
};
}  // namespace hid::page

#endif  // HID_PAGE_VENDOR1_HPP
