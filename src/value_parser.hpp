#pragma once

#include "registry.hpp"

#include <memory>

namespace zizany {
    class stream_parser;

    class unity_file_reference;

    class unity_value;

    class value_parser {
    public:
        virtual ~value_parser();

        virtual std::unique_ptr<unity_value> parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end) = 0;
    };
}
