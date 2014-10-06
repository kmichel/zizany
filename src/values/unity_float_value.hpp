#pragma once

#include "unity_value.hpp"

namespace zizany {
    class unity_float_value : public unity_value {
    public:
        float value;

        unity_float_value(float value_);

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value_) const;

        // XXX: we should also implement equality with integers

        virtual bool equals(float value_) const;
    };

}
