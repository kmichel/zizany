#include "unity_composite_value.hpp"

#include "../unity_type.hpp"

namespace zizany {
    unity_composite_value::unity_composite_value(const unity_type &type_)
            : unity_value(type_), members() {
    }

    void
    unity_composite_value::print(json_writer &writer) const {
        members.print(writer);
    }
}
