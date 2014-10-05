#pragma once

#include "../owning_vector.hpp"
#include "unity_value.hpp"

namespace zizany {
    class unity_composite_value : public unity_value {
    public:
        owning_vector<unity_value> members;

        unity_composite_value(unity_type &type_, unity_value *parent_);

        virtual void print(json_writer &writer) const;
    };
}