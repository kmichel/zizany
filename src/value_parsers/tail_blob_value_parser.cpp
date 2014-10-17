#include "tail_blob_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../values/unity_blob_value.hpp"

namespace zizany {
    std::unique_ptr<unity_value>
    tail_blob_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &/*file_references*/, const std::int64_t expected_end) {
        const std::int64_t leftover = expected_end - parser.tell();
        std::unique_ptr<unity_blob_value> blob(new unity_blob_value);
        parser.parse(blob->data, static_cast<std::size_t>(leftover));
        return std::move(blob);
    }
}
