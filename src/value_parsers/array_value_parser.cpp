#include "array_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../values/unity_array_value.hpp"

namespace zizany {
    array_value_parser::array_value_parser(std::unique_ptr<value_parser> &&element_parser_)
            : value_parser(), element_parser(std::move(element_parser_)) {
    }

    std::unique_ptr<unity_value>
    array_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end) {
        std::unique_ptr<unity_array_value> value(new unity_array_value);
        const std::uint32_t length(parser.parse<std::uint32_t>());
        if (length > 0) {
            value->elements.reserve(length);
            for (std::uint32_t index = 0; index < length; ++index)
                value->elements.add(element_parser->parse_value(parser, file_references, expected_end));
        }
        return std::move(value);
    }
}
