#pragma once

#include "../value_parser.hpp"

namespace zizany {
    class array_value_parser : public value_parser {
        std::unique_ptr<value_parser> element_parser;
    public:
        array_value_parser(std::unique_ptr<value_parser> &&element_parser_);

        virtual std::unique_ptr<unity_value> parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end);
    };
}
