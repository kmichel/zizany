#include "unity_array_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_array_value::unity_array_value(const unity_type &type_, unity_value *parent_)
            : unity_value(type_, parent_), elements() {
    }

    void
    unity_array_value::print(json_writer &writer) const {
        writer.start_array();
        for (const unity_value &element : elements)
            element.print(writer);
        writer.end_array();
    }
}
