#include "unity_bool_value.hpp"

#include "../json_writer.hpp"

namespace zizany {

    unity_bool_value::unity_bool_value(bool value_)
            : unity_value(), value(value_) {
    }

    void
    unity_bool_value::print(json_writer &writer) const {
        writer.add_bool(value);
    }
}
