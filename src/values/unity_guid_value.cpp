#include "unity_guid_value.hpp"

namespace zizany {
    unity_guid_value::unity_guid_value()
            : unity_value(), value() {
    }

    void
    unity_guid_value::print(json_writer &writer) const {
        value.print(writer);
    }

    bool
    unity_guid_value::equals(const unity_value &value_) const {
        return value_.equals(value);
    }

    bool
    unity_guid_value::equals(const guid &value_) const {
        return value_ == value;
    }
}
