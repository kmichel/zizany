#include "unity_string_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_string_value::unity_string_value()
            : unity_value(), chars() {
    }

    void
    unity_string_value::print(json_writer &writer) const {
        writer.add_string(chars.data(), chars.size());
    }

    bool
    unity_string_value::equals(const unity_value &value) const {
        // XXX: string creation
        return value.equals(std::string(chars.data(), chars.size()));
    }

    bool
    unity_string_value::equals(const std::string &value) const {
        // XXX: string creation
        return std::string(chars.data(), chars.size()) == value;
    }
}
