#include "blob_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../values/unity_blob_value.hpp"

namespace zizany {
    std::unique_ptr<unity_value>
    blob_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &/*file_references*/, const std::int64_t /*expected_end*/) {
        std::unique_ptr<unity_blob_value> value(new unity_blob_value);
        const std::uint32_t length(parser.parse<std::uint32_t>());
        if (length > 0)
            parser.parse(value->data, length);
        return std::move(value);
    }
}
