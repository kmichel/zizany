#include "unity_composite_value.hpp"

#include "../json_writer.hpp"
#include "../unity_type.hpp"

namespace zizany {
    unity_composite_value::unity_composite_value(unity_type &type_, unity_value *parent_)
            : unity_value(type_, parent_), members() {
    }

    void
    unity_composite_value::print(json_writer &writer) const {
        writer.start_object();
        for (std::size_t index = 0; index < members.size(); ++index) {
            const unity_value &member(members.at(index));
            writer.add_key(member.type.member_name);
            member.print(writer);
        }
        writer.end_object();
    }
}
