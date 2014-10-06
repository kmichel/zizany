#include "unity_guid_value.hpp"

namespace zizany {
    unity_guid_value::unity_guid_value(const unity_type &type_)
            : unity_value(type_) {
    }

    void
    unity_guid_value::print(json_writer &writer) const {
        value.print(writer);
    }
}
