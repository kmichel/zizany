#include "unity_string_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_string_value::unity_string_value(const unity_type &type_)
            : unity_value(type_), chars() {
    }

    void
    unity_string_value::print(json_writer &writer) const {
        writer.add_string(chars.data(), chars.size());
    }
}
