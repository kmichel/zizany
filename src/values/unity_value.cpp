#include "unity_value.hpp"

#include "unity_array_value.hpp"
#include "unity_composite_value.hpp"
#include "../delta_store.hpp"

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
    unity_value::equals(const std::string &/*value*/) const {
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
    unity_value::member_equals(const std::string &/*member_name*/, const unity_value &/*value*/) const {
        return false;
    }

    bool
    unity_value::size_equals(std::size_t /*size*/) const {
        return false;
    }

    bool
    unity_value::item_equals(std::size_t /*index*/, const unity_value &/*value*/) const {
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
