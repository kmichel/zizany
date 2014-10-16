#include "unity_double_value.hpp"

#include "../json_writer.hpp"

#include <cmath>

namespace zizany {
    unity_double_value::unity_double_value(const double value_)
            : unity_value(), value(value_) {
    }

    void
    unity_double_value::print(json_writer &writer) const {
        writer.add_number(value);
    }

    bool
    unity_double_value::equals(const unity_value &value_) const {
        return value_.equals(value);
    }

    bool
    unity_double_value::equals(const double value_) const {
        // We really want to compare doubles exactly
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
        // XXX: nan representation may differ, which should trigger a diff
        return value_ == value || (std::isnan(value_) && std::isnan(value));
#pragma GCC diagnostic pop
    }
}
