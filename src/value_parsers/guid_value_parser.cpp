#include "guid_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../values/unity_guid_value.hpp"

namespace zizany {
    std::unique_ptr<unity_value>
    guid_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &/*file_references*/, const std::int64_t /*expected_end*/) {
        std::unique_ptr<unity_guid_value> value(new unity_guid_value());
        value->value.a = parser.parse<std::uint32_t>();
        value->value.b = parser.parse<std::uint32_t>();
        value->value.c = parser.parse<std::uint32_t>();
        value->value.d = parser.parse<std::uint32_t>();
        return std::move(value);
    }
}
