#include "unity_composite_value.hpp"

#include "../json_writer.hpp"
#include "../unity_type.hpp"

namespace zizany {
    unity_composite_value::unity_composite_value(const unity_type &type_)
            : unity_value(type_), members() {
    }

    void
    unity_composite_value::print(json_writer &writer) const {
        writer.start_object();
        for (std::size_t member_index = 0; member_index < type.members.size(); ++member_index) {
            writer.add_key(type.members.at(member_index).name);
            members.at(member_index).print(writer);
        }
        writer.end_object();
    }
}
