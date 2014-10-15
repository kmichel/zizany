#include "unity_value.hpp"

#include "diff/delta_store.hpp"
#include "values/unity_array_value.hpp"
#include "values/unity_composite_value.hpp"

namespace zizany {
    unity_value::unity_value() {
    }

    unity_value::~unity_value() {
    }

    bool
    unity_value::equals(bool /*value*/) const {
        return false;
    }

    bool
    unity_value::equals(char /*value*/) const {
        return false;
    }

    bool
    unity_value::equals(std::int64_t /*value*/) const {
        return false;
    }

    bool
    unity_value::equals(std::uint64_t /*value*/) const {
        return false;
    }

    bool
    unity_value::equals(float /*value*/) const {
        return false;
    }

    bool
    unity_value::equals(double /*value*/) const {
        return false;
    }

    bool
    unity_value::equals(const guid &/*value*/) const {
        return false;
    }

    bool
    unity_value::equals(const asset_reference &/*value*/) const {
        return false;
    }

    bool
    unity_value::equals(const std::vector<char> &/*value*/) const {
        return false;
    }

    bool
    unity_value::equals(const unity_array_value &/*value*/) const {
        return false;
    }

    bool
    unity_value::equals(const unity_composite_value &/*value*/) const {
        return false;
    }

    void
    unity_value::compare(const unity_value &value, delta_store &store) const {
        if (!equals(value))
            store.modify_value(*this, value);
    }

    void
    unity_value::compare(const unity_array_value &value, delta_store &store) const {
        if (!equals(value))
            store.modify_value(*this, value);
    }

    void
    unity_value::compare(const unity_composite_value &value, delta_store &store) const {
        if (!equals(value))
            store.modify_value(*this, value);
    }
}
