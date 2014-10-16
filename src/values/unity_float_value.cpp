#include "unity_float_value.hpp"

#include "../json_writer.hpp"

#include <cmath>

namespace zizany {
    unity_float_value::unity_float_value(const float value_)
            : unity_value(), value(value_) {
    }

    void
    unity_float_value::print(json_writer &writer) const {
        writer.add_number(value);
    }

    bool
    unity_float_value::equals(const unity_value &value_) const {
        return value_.equals(value);
    }

    bool
    unity_float_value::equals(const float value_) const {
        // We really want to compare floats exactly
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
        // XXX: nan representation may differ, which should trigger a diff
        return value_ == value || (std::isnan(value_) && std::isnan(value));
#pragma GCC diagnostic pop
    }
}
