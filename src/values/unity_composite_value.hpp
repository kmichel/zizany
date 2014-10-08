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
    };
}
