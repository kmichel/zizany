#pragma once

#include "../stream_parser.hpp"
#include "../value_parser.hpp"
#include "../values/unity_integer_value.hpp"

namespace zizany {
    template<typename value_type>
    class integer_value_parser : public value_parser {
    public:
        virtual std::unique_ptr<unity_value> parse_value(stream_parser &parser, const registry<unity_file_reference> &/*file_references*/, const std::int64_t /*expected_end*/) {
            return std::unique_ptr<unity_value>(
                    new unity_integer_value<value_type>(parser.parse<value_type>()));
        }
    };
}
