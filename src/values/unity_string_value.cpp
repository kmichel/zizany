#include "unity_string_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_string_value::unity_string_value()
            : unity_blob_value() {
    }

    void
    unity_string_value::print(json_writer &writer) const {
        writer.add_string(data.data(), data.size());
    }
}
