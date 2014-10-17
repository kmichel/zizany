#include "double_value_parser.hpp"

#include "../stream_parser.hpp"
#include "../values/unity_double_value.hpp"

namespace zizany {
    std::unique_ptr<unity_value>
    double_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &/*file_references*/, const std::int64_t /*expected_end*/) {
        return std::unique_ptr<unity_value>(
                new unity_double_value(parser.parse<double>()));
    }
}
