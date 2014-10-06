#include "unity_bool_array_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_bool_array_value::unity_bool_array_value(const unity_type &type_)
            : unity_value(type_), elements() {
    }

    void unity_bool_array_value::print(json_writer &writer) const {
        writer.start_array(/*force_inline:*/true);
        for (bool element : elements)
            writer.add_bool(element);
        writer.end_array();
    }
}
