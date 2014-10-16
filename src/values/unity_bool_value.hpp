#pragma once

#include "../unity_value.hpp"

namespace zizany {
    class unity_bool_value : public unity_value {
    public:
        bool value;

        unity_bool_value(const bool value_);

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value_) const;

        virtual bool equals(const bool value_) const;
    };
}
