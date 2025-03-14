#ifndef HID_RDF_REPORT_PARSER_HPP_
#define HID_RDF_REPORT_PARSER_HPP_

#include "vendor.hpp"
#include "vendor1.hpp"

#include <algorithm>
#include <array>
#include "hid/rdf/global_items.hpp"
#include "hid/rdf/parser.hpp"
#include "hid/report.hpp"
#include "hid/report_array.hpp"
#include "hid/report_bitset.hpp"

#include "hid/page/button.hpp"
#include "hid/page/game.hpp"
#include "hid/page/generic_desktop.hpp"

namespace hid {
struct report_parser {
  using descriptor_view_type = rdf::descriptor_view;
  using size_type = std::uint16_t;

  /// @brief This class parses the HID report descriptor, gathering all report
  /// size
  ///        and TLC assignment information, and verifying that the descriptor
  ///        describes a valid HID protocol.
  class parser final : public rdf::parser<descriptor_view_type::iterator> {
   public:
    using base = rdf::parser<descriptor_view_type::iterator>;

    constexpr explicit parser(const descriptor_view_type& desc_view) : base() {
      parse_items(desc_view);

      // when finished, run a final check that each report has byte size
      for (const auto& sizes : report_bit_sizes_) {
        for (const auto& size : sizes) {
          HID_RDF_ASSERT(size % 8 == 0, ex_report_total_size_invalid);
        }
      }
    }

    [[nodiscard]] constexpr size_type max_report_size(
        const report::type type) const {
      if (!uses_report_ids()) {
        return bit_size(type, 0) / 8;
      }
      auto& report_sizes = bit_sizes_by_type(type);
      auto begin = report_sizes.begin();
      if (const auto max_size =
              *std::max_element(++begin, report_sizes.end()) / 8;
          max_size > 0) {
        return sizeof(report::id) + max_size;
      }
      return 0;
    }

    [[nodiscard]] constexpr bool uses_report_ids() const {
      return max_report_id() > 0;
    }

    [[nodiscard]] constexpr report::id::type max_report_id() const {
      return *std::ranges::max_element(max_report_ids_);
    }

    [[nodiscard]] constexpr report::id::type max_report_id(
        report::type type) const {
      return max_report_ids_[static_cast<size_t>(type) - 1];
    }

   private:
    /// @brief  The override of this method is meant to handle the collection
    /// begins of the descriptor.
    /// @param  collection: the type of collection
    /// @param  global_state: the current global items state
    /// @param  main_section: the span of items between the previous and this
    /// main item, for local items parsing
    /// @param  tlc_number: the Top Level Collection index, where this item is
    /// found (incremented before this method is called)
    /// @return CONTINUE to continue the parsing until the next main item,
    ///         or BREAK to terminate it early
    constexpr control parse_collection_begin(
        const rdf::main::collection_type collection,
        const rdf::global_item_store& global_state,
        const items_view_type& main_section,
        unsigned tlc_number) override {
      if (collection == rdf::main::collection_type::REPORT) {
        spdlog::debug("(collection) REPORT");
      } else if (collection == rdf::main::collection_type::APPLICATION) {
        spdlog::debug("(collection) APPLICATION");
      } else if (collection == rdf::main::collection_type::PHYSICAL) {
        spdlog::debug("(collection) PHYSICAL");
      } else if (collection == rdf::main::collection_type::LOGICAL) {
        spdlog::debug("(collection) LOGICAL");
      } else if (collection == rdf::main::collection_type::NAMED_ARRAY) {
        spdlog::debug("(collection) NAMED_ARRAY");
      } else if (collection == rdf::main::collection_type::USAGE_SWITCH) {
        spdlog::debug("(collection) USAGE_SWITCH");
      } else if (collection == rdf::main::collection_type::USAGE_MODIFIER) {
        spdlog::debug("(collection) USAGE_MODIFIER");
      } else if (collection ==
                 rdf::main::collection_type::VENDOR_DEFINED_BEGIN) {
        spdlog::debug("(collection) VENDOR_DEFINED_BEGIN");
      }

      for (auto& it : main_section) {
        if (it.has_tag(rdf::local::tag::USAGE)) {
          auto usage = get_usage(it, global_state);
          parse_usage("main", usage.page_id(), usage.id());
        }
      }

      return control::CONTINUE;
    }

    /// @brief  The override of this method is meant to handle the collection
    /// ends of the descriptor.
    /// @param  global_state: the current global items state
    /// @param  main_section: the span of items between the previous and this
    /// main item, for local items parsing
    /// @param  tlc_number: the Top Level Collection index, where this item is
    /// found
    /// @return CONTINUE to continue the parsing until the next main item,
    ///         or BREAK to terminate it early
    constexpr control parse_collection_end(
        [[maybe_unused]] const rdf::global_item_store& global_state,
        [[maybe_unused]] const items_view_type& main_section,
        [[maybe_unused]] unsigned tlc_number) override {
      spdlog::debug("COLLECTION END");

      return control::CONTINUE;
    }

    static constexpr void parse_usage(const char* section,
                                      const page_id_t page_id,
                                      const usage_id_t id) {
      if (page_id == page::info<page::generic_desktop>::page_id) {
        spdlog::debug("\t({}) {}", section,
                      page::info<page::generic_desktop>::name);
        if (id) {
          if (const auto val = page::info<page::generic_desktop>::get(id);
              val == page::generic_desktop::X)
            spdlog::info("\t\tX");
          else if (val == page::generic_desktop::Y)
            spdlog::info("\t\tY");
          else if (val == page::generic_desktop::Z)
            spdlog::info("\t\tZ");
          else if (val == page::generic_desktop::RX)
            spdlog::info("\t\tRX");
          else if (val == page::generic_desktop::RY)
            spdlog::info("\t\tRY");
          else if (val == page::generic_desktop::RZ)
            spdlog::info("\t\tRZ");
        } else if (page_id == page::info<page::game>::page_id) {
          spdlog::debug("\t(main) {}", page::info<page::game>::name);
          if (const auto val = page::info<page::game>::get(id);
              val == page::game::_3D_GAME_CONTROLLER) {
            spdlog::info("\t\t3D_GAME_CONTROLLER");
          }
        } else if (page_id == page::info<page::button>::page_id) {
          spdlog::debug("\t(main) {} {}", page::info<page::button>::name, id);
        } else if (page_id == page::info<page::vendor>::page_id) {
          spdlog::debug("\t(main) {} 0x{:02X}", page::info<page::vendor>::name,
                        id);
        } else if (page_id == page::info<page::vendor1>::page_id) {
          spdlog::debug("\t(main) {} 0x{:02X}", page::info<page::vendor1>::name,
                        id);
        } else {
          spdlog::debug("\t(main) USAGE_PAGE: 0x{:02X} {}", page_id,
                        (id > 0 ? std::to_string(id) : ""));
        }
      }
    }

    /// @brief  The override of this function is meant to handle
    /// the main data field items
    ///         (INPUT/OUTPUT/FEATURE) of the descriptor.
    /// @param  item: the current main item that needs parsing
    /// @param  global_state: the current global items state
    /// @param  main_section: the span of items between the
    /// previous and this main item, for local items parsing
    /// @param  tlc_number: the Top Level Collection index, where
    /// this item is found
    /// @return CONTINUE to continue the parsing until the next
    /// main item,
    ///         or BREAK to terminate it early
    constexpr control parse_report_data_field(
        const item_type& item,
        const rdf::global_item_store& global_state,
        [[maybe_unused]] const items_view_type& main_section,
        const unsigned tlc_number) override {
      using namespace hid::rdf;

      report::type rtype = tag_to_report_type(item.main_tag());

      // get report ID (or use 0 if not present)
      report::id::type report_id = 0;

      if (global_state.has_item(global::tag::REPORT_ID)) {
        if (const auto report_id_item =
                global_state.get_item(global::tag::REPORT_ID);
            report_id_item && report_id_item->has_data()) {
          report_id = report_id_item->value_unsigned();
          spdlog::debug("\t(global) REPORT_ID: 0x{:02X}", report_id);

          // report ID verification
          HID_RDF_ASSERT(report_id >= report::id::min(), ex_report_id_zero);
          HID_RDF_ASSERT(report_id <= report::id::max(), ex_report_id_excess);
          if (!uses_report_ids()) {
            for (auto& sizes : report_bit_sizes_) {
              HID_RDF_ASSERT(sizes[0] == 0, ex_report_id_missing);
            }
          }
          auto& rid = max_report_ids_[static_cast<size_t>(rtype) - 1];
          rid = std::max(rid, report_id);
        }
      }

      if (global_state.has_item(global::tag::USAGE_PAGE)) {
        if (const auto* usage_page =
                global_state.get_item(global::tag::USAGE_PAGE);
            usage_page && usage_page->has_data())
          parse_usage("global", usage_page->value_unsigned(), 0);
      }

      for (auto& it : main_section) {
        if (it.has_tag(local::tag::USAGE)) {
          auto usage = get_usage(it, global_state);
          parse_usage("main", usage.page_id(), usage.id());
        } else if (it.has_tag(rdf::local::tag::USAGE_MINIMUM)) {
          spdlog::debug("\t(local) USAGE_MINIMUM: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::USAGE_MAXIMUM)) {
          spdlog::debug("\t(local) USAGE_MAXIMUM: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::DESIGNATOR_INDEX)) {
          spdlog::debug("\t(local) DESIGNATOR_INDEX: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::DESIGNATOR_MINIMUM)) {
          spdlog::debug("\t(local) DESIGNATOR_MINIMUM: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::DESIGNATOR_MAXIMUM)) {
          spdlog::debug("\t(local) DESIGNATOR_MAXIMUM: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::STRING_INDEX)) {
          spdlog::debug("\t(local) STRING_INDEX: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::STRING_MINIMUM)) {
          spdlog::debug("\t(local) STRING_MINIMUM: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::STRING_MAXIMUM)) {
          spdlog::debug("\t(local) STRING_MAXIMUM: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        } else if (it.has_tag(rdf::local::tag::DELIMITER)) {
          spdlog::debug("\t(local) DELIMITER: 0x{:02X} {}{}",
                        it.value_unsigned(), it.is_long() ? "long" : "",
                        it.is_short() ? "short" : "");
        }
      }

      switch (rtype) {
        case report::type::INPUT: {
          spdlog::debug("\t(main) INPUT");
          break;
        }
        case report::type::OUTPUT: {
          spdlog::debug("\t(main) OUTPUT");
          break;
        }
        case report::type::FEATURE: {
          spdlog::debug("\t(main) FEATURE");
          break;
        }
      }

      // get the items defining the size of this/these report data
      // elements
      const auto* report_size_item =
          global_state.get_item(global::tag::REPORT_SIZE);
      HID_RDF_ASSERT(report_size_item != nullptr, ex_report_size_missing);
      const auto report_size = report_size_item->value_unsigned();
      HID_RDF_ASSERT(report_size > 0, ex_report_size_zero);
      spdlog::debug("\t(global) REPORT_SIZE: {}", report_size);

      const auto* report_count_item =
          global_state.get_item(global::tag::REPORT_COUNT);
      HID_RDF_ASSERT(report_count_item != nullptr, ex_report_count_missing);
      const auto report_count = report_count_item->value_unsigned();
      HID_RDF_ASSERT(report_count > 0, ex_report_count_zero);
      spdlog::debug("\t(global) REPORT_COUNT: {}", report_count);

      if (global_state.has_item(global::tag::LOGICAL_MINIMUM)) {
        const auto* logical_min =
            global_state.get_item(global::tag::LOGICAL_MINIMUM);
        if (logical_min && logical_min->has_data()) {
          spdlog::debug("\t(global) LOGICAL_MINIMUM: {} {}{}",
                        logical_min->value_unsigned(),
                        logical_min->is_long() ? "long" : "",
                        logical_min->is_short() ? "short" : "");
        }
      }

      if (global_state.has_item(global::tag::LOGICAL_MAXIMUM)) {
        const auto* logical_max =
            global_state.get_item(global::tag::LOGICAL_MAXIMUM);
        if (logical_max && logical_max->has_data()) {
          spdlog::debug("\t(global) LOGICAL_MAXIMUM: {} {}{}",
                        logical_max->value_unsigned(),
                        logical_max->is_long() ? "long" : "",
                        logical_max->is_short() ? "short" : "");
        }
      }

      if (global_state.has_item(global::tag::PHYSICAL_MINIMUM)) {
        const auto* physical_min =
            global_state.get_item(global::tag::PHYSICAL_MINIMUM);
        if (physical_min && physical_min->has_data()) {
          spdlog::debug("\t(global) PHYSICAL_MINIMUM: {} {}{}",
                        physical_min->value_unsigned(),
                        physical_min->is_long() ? "long" : "",
                        physical_min->is_short() ? "short" : "");
        }
      }

      if (global_state.has_item(global::tag::PHYSICAL_MAXIMUM)) {
        const auto* physical_max =
            global_state.get_item(global::tag::PHYSICAL_MAXIMUM);
        if (physical_max && physical_max->has_data()) {
          spdlog::debug("\t(global) PHYSICAL_MAXIMUM: {} {}{}",
                        physical_max->value_unsigned(),
                        physical_max->is_long() ? "long" : "",
                        physical_max->is_short() ? "short" : "");
        }
      }

      if (const auto* unit_exp =
              global_state.get_item(global::tag::UNIT_EXPONENT);
          unit_exp && unit_exp->has_data()) {
        spdlog::debug("\t(global) UNIT_EXPONENT: {} {}{}",
                      unit_exp->value_unsigned(),
                      unit_exp->is_long() ? "long" : "",
                      unit_exp->is_short() ? "short" : "");
      }

      if (global_state.has_item(global::tag::UNIT)) {
        if (const auto* unit = global_state.get_item(global::tag::UNIT);
            unit && unit->has_data()) {
          spdlog::debug("\t(global) UNIT: 0x{:02X} {}{}",
                        unit->value_unsigned(), unit->is_long() ? "long" : "",
                        unit->is_short() ? "short" : "");
        }
      }

      if constexpr (global_state.has_item(global::tag::PUSH)) {
        if (const auto* push = global_state.get_item(global::tag::PUSH);
            push && push->has_data())
          spdlog::debug("\t(global) PUSH: {}", push->value_unsigned());
      }

      if constexpr (global_state.has_item(global::tag::POP)) {
        if (const auto* pop = global_state.get_item(global::tag::POP);
            pop && pop->has_data())
          spdlog::debug("\t(global) POP: {}", pop->value_unsigned());
      }

      // increase size of this report
      bit_size(rtype, report_id) += report_size * report_count;

      // verify that the report doesn't cross TLC boundary
      if (auto& report_tlc_index = tlc_index(rtype, report_id);
          report_tlc_index == 0) {
        // first piece of the report, assign to TLC now
        report_tlc_index = tlc_number;
      } else {
        HID_RDF_ASSERT(report_tlc_index == tlc_number,
                       ex_report_crossing_tlc_bounds);
      }
      return control::CONTINUE;
      return control::CONTINUE;
    }

    constexpr size_type& bit_size(const report::type rt,
                                  const report::id::type id) {
      return bit_sizes_by_type(rt)[id];
    }
    [[nodiscard]] constexpr const size_type& bit_size(
        const report::type rt,
        const report::id::type id) const {
      return bit_sizes_by_type(rt)[id];
    }
    constexpr std::array<size_type, report::id::max()>& bit_sizes_by_type(
        report::type rt) {
      return report_bit_sizes_[static_cast<report::id::type>(rt) - 1];
    }
    [[nodiscard]] constexpr const std::array<size_type, report::id::max()>&
    bit_sizes_by_type(report::type rt) const {
      return report_bit_sizes_[static_cast<report::id::type>(rt) - 1];
    }

    constexpr unsigned& tlc_index(const report::type rt,
                                  const report::id::type id) {
      return tlc_indexes_by_type(rt)[id];
    }
    [[nodiscard]] constexpr const unsigned& tlc_index(
        const report::type rt,
        const report::id::type id) const {
      return tlc_indexes_by_type(rt)[id];
    }
    constexpr std::array<unsigned, report::id::max()>& tlc_indexes_by_type(
        report::type rt) {
      return report_tlc_indexes_[static_cast<report::id::type>(rt) - 1];
    }
    [[nodiscard]] constexpr const std::array<unsigned, report::id::max()>&
    tlc_indexes_by_type(report::type rt) const {
      return report_tlc_indexes_[static_cast<report::id::type>(rt) - 1];
    }

    /// You need to replace the internal logic to push a new item
    /// to a vector of HID report items, filled with the
    /// information you get out of the passed parameters. The
    /// vector is a member variable of your custom parser, that's
    /// how it can be accessed reasonably in this virtual
    /// function. You get the boolean values out of the main_item
    /// parameter, the usages are collected by iterating through
    /// the main_section (see also the aforementioned
    /// get_application_usage_id), the rest are acquired from
    /// global_state.

    std::array<std::array<size_type, report::id::max()>, 3> report_bit_sizes_{};
    std::array<std::array<unsigned, report::id::max()>, 3>
        report_tlc_indexes_{};
    std::array<report::id::type, 3> max_report_ids_{};
  };
};
}  // namespace hid

#endif  // HID_RDF_REPORT_PARSER_HPP_
