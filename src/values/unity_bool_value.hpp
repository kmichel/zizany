#pragma once

#include "unity_value.hpp"

namespace zizany {
    class unity_bool_value : public unity_value {
    public:
        bool value;

        unity_bool_value(const unity_type &type_, bool value_);

        virtual void print(json_writer &writer) const;
    };
}
