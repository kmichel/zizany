#pragma once

#include "unity_blob_value.hpp"

namespace zizany {
    class unity_string_value : public unity_blob_value {
    public:
        unity_string_value();

        virtual void print(json_writer &writer) const;
    };
}
