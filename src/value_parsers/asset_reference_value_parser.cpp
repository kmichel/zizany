#include "asset_reference_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../unity_file_reference.hpp"
#include "../values/unity_asset_reference_value.hpp"

namespace zizany {
    std::unique_ptr<unity_value>
    asset_reference_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t /*expected_end*/) {
        std::unique_ptr<unity_asset_reference_value> asset_reference_value(new unity_asset_reference_value());
        const std::int32_t file_reference_id = parser.parse<std::int32_t>();
        if (file_reference_id == 0) {
            asset_reference_value->value.is_local = true;
        } else {
            asset_reference_value->value.is_local = false;
            asset_reference_value->value.file_guid = file_references.get_by_id(file_reference_id).file_guid;
        }
        asset_reference_value->value.asset_id = parser.parse<std::int32_t>();
        return std::move(asset_reference_value);
    }
}
