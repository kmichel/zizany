#include "fixed_size_blob_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../values/unity_blob_value.hpp"

namespace zizany {
    fixed_size_blob_value_parser::fixed_size_blob_value_parser(const std::size_t size_)
            : size(size_) {
    }

    std::unique_ptr<unity_value>
    fixed_size_blob_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &/*file_references*/, const std::int64_t /*expected_end*/) {
        std::unique_ptr<unity_blob_value> value(new unity_blob_value);
        parser.parse(value->data, size);
        return std::move(value);
    }
}
