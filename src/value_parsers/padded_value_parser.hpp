#pragma once

#include "../value_parser.hpp"

namespace zizany {
    class padded_value_parser : public value_parser {
        std::unique_ptr<value_parser> delegate_parser;
    public:
        padded_value_parser(std::unique_ptr<value_parser> &&delegate_parser_);

        virtual std::unique_ptr<unity_value> parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end);
    };
}
