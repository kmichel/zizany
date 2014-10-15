#pragma once

#include "../unity_value.hpp"

namespace zizany {
    class unity_double_value : public unity_value {
    public:
        double value;

        unity_double_value(double value_);

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value_) const;

        // XXX: we should also implement equality with integers

        virtual bool equals(double value_) const;
    };

}
