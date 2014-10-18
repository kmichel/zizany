#pragma once

#include "unity_string_value.hpp"

namespace zizany {
    class unity_multiline_string_value : public unity_string_value {
    public:
        unity_multiline_string_value();

        virtual void print(json_writer &writer) const;
    };
}
