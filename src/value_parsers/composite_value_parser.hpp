#pragma once

#include "../owning_vector.hpp"
#include "../value_parser.hpp"

namespace zizany {
    class composite_value_parser : public value_parser {
        std::vector<std::string> member_names;
        owning_vector<value_parser> member_parsers;
    public:
        composite_value_parser();

        void reserve(const std::size_t size);

        void add_member(const std::string &member_name, std::unique_ptr<value_parser> &&member_parser);

        virtual std::unique_ptr<unity_value> parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end);
    };
}
