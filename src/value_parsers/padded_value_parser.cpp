#include "padded_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../unity_value.hpp"

namespace zizany {
    padded_value_parser::padded_value_parser(std::unique_ptr<value_parser> &&delegate_parser_)
            : value_parser(), delegate_parser(std::move(delegate_parser_)) {
    }

    std::unique_ptr<unity_value>
    padded_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end) {
        std::unique_ptr<unity_value> value(delegate_parser->parse_value(parser, file_references, expected_end));
        parser.align(4);
        return std::move(value);
    }
}
