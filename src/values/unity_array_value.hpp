#pragma once

#include "unity_value.hpp"
#include "../owning_vector.hpp"

namespace zizany {
    class unity_array_value : public unity_value {
    public:
        owning_vector<unity_value> elements;

        unity_array_value(unity_type &type_, unity_value *parent_);

        virtual void print(json_writer &writer) const;
    };
}