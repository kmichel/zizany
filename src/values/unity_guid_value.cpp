#include "unity_guid_value.hpp"

namespace zizany {
    unity_guid_value::unity_guid_value()
            : unity_value(), value() {
    }

    void
    unity_guid_value::print(json_writer &writer) const {
        value.print(writer);
    }
}
