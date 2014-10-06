#include "unity_bool_value.hpp"

#include "../json_writer.hpp"

namespace zizany {

    unity_bool_value::unity_bool_value(const unity_type &type_, bool value_)
            : unity_value(type_), value(value_) {
    }

    void
    unity_bool_value::print(json_writer &writer) const {
        writer.add_bool(value);
    }
}
