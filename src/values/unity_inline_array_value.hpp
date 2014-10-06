#pragma once

#include "unity_array_value.hpp"

namespace zizany {
    class unity_inline_array_value : public unity_array_value {
    public:
        unity_inline_array_value();

        virtual void print(json_writer &writer) const;
    };
}
