#pragma once

#include "../unity_value_members.hpp"
#include "../owning_vector.hpp"
#include "unity_value.hpp"

namespace zizany {
    class unity_composite_value : public unity_value {
    public:
        unity_value_members members;

        unity_composite_value();

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value) const;

        virtual bool equals(const unity_composite_value &value) const;

        virtual void compare(const unity_value &value, delta_store &store) const;

        virtual void compare(const unity_composite_value &value, delta_store &store) const;

    };
}
