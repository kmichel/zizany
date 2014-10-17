#include "multiline_string_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../values/unity_multiline_string_value.hpp"

namespace zizany {
    std::unique_ptr<unity_value>
    multiline_string_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &/*file_references*/, const std::int64_t /*expected_end*/) {
        std::unique_ptr<unity_multiline_string_value> value(new unity_multiline_string_value);
        const std::uint32_t length(parser.parse<std::uint32_t>());
        if (length > 0) {
            parser.parse(value->data, length);
            parser.align(4);
        }
        return std::move(value);
    }
}
