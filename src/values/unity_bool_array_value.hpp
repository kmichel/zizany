#pragma once

#include "unity_value.hpp"

#include <vector>

namespace zizany {
    class unity_bool_array_value : public unity_value {
    public:
        std::vector<bool> elements;

        unity_bool_array_value(const unity_type &type_);

        virtual void print(json_writer &writer) const;
    };
}
