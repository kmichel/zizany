#include "unity_asset_reference_value.hpp"

namespace zizany {
    unity_asset_reference_value::unity_asset_reference_value()
            : unity_value(), value() {
    }

    void
    unity_asset_reference_value::print(json_writer &writer) const {
        value.print(writer);
    }

    bool
    unity_asset_reference_value::equals(const unity_value &value_) const {
        return value_.equals(value);
    }

    bool
    unity_asset_reference_value::equals(const asset_reference &value_) const {
        return value_ == value;
    }
}
