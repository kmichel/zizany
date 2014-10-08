#include "unity_array_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_array_value::unity_array_value()
            : unity_value(), elements() {
    }

    void
    unity_array_value::print(json_writer &writer) const {
        writer.start_array();
        for (const unity_value &element : elements)
            element.print(writer);
        writer.end_array();
    }
}
