#include "composite_value_parser.hpp"

#include "../values/unity_composite_value.hpp"

namespace zizany {
    composite_value_parser::composite_value_parser()
            : value_parser(), member_names(), member_parsers() {
    }

    void
    composite_value_parser::reserve(const std::size_t size) {
        member_names.reserve(size);
        member_parsers.reserve(size);
    }

    void
    composite_value_parser::add_member(const std::string &member_name, std::unique_ptr<value_parser> &&member_parser) {
        member_names.push_back(member_name);
        member_parsers.add(std::move(member_parser));
    }

    std::unique_ptr<unity_value>
    composite_value_parser::parse_value(stream_parser &parser, const registry<unity_file_reference> &file_references, const std::int64_t expected_end) {
        std::unique_ptr<unity_composite_value> value(new unity_composite_value);
        const std::size_t members_count(member_names.size());
        value->members.reserve(members_count);
        for (std::size_t index = 0; index < members_count; ++index)
            value->members.add(member_names.at(index), member_parsers.at(index).parse_value(parser, file_references, expected_end));
        return std::move(value);
    }
}
