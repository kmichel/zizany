#pragma once

#include "../value_parser.hpp"

namespace zizany {
    class fixed_size_blob_value_parser : public value_parser {
        const std::size_t size;
    public:
        fixed_size_blob_value_parser(const std::size_t size);

        virtual std::unique_ptr<unity_value> parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end);
    };
}
