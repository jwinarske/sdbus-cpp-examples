#ifndef HID_PAGE_VENDOR_HPP
#define HID_PAGE_VENDOR_HPP

#include "hid/usage.hpp"

namespace hid::page {
enum class vendor : std::uint16_t;

template <>
struct info<vendor> {
  constexpr static page_id_t page_id = 0xFF00;
  constexpr static usage_id_t max_usage_id = 10;
  constexpr static auto name = "Vendor Defined";
};

enum class vendor : std::uint16_t {
  USB_1 = 0x20,
  USB_2 = 0x21,
  USB_3 = 0x22,
  USB_4 = 0x23,
  USB_5 = 0x24,
  USB_6 = 0x25,
  USB_7 = 0x26,
  USB_8 = 0x27,

  UNKNOWN_1 = 0x31,
  UNKNOWN_2 = 0x3B,
  UNKNOWN_3 = 0x32,
  UNKNOWN_4 = 0x33,
  UNKNOWN_5 = 0x34,
  UNKNOWN_6 = 0x35,
  UNKNOWN_7 = 0x36,
  UNKNOWN_8 = 0x37,
  UNKnOWN_9 = 0x38,
  UNKNOWN_10 = 0x39,

  UNKNOWN_11 = 0x40,
};
}  // namespace hid::page

#endif  // HID_PAGE_VENDOR_HPP
