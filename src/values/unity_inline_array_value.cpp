#include "unity_inline_array_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_inline_array_value::unity_inline_array_value()
            : unity_array_value() {
    }

    void
    unity_inline_array_value::print(json_writer &writer) const {
        writer.start_array(/*inline:*/true);
        for (const unity_value &element : elements)
            element.print(writer);
        writer.end_array();
    }
}
