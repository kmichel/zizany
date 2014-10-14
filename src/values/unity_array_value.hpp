#pragma once

#include "unity_value.hpp"
#include "../owning_vector.hpp"

namespace zizany {
    class unity_array_value : public unity_value {
    public:
        owning_vector<unity_value> elements;

        unity_array_value();

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value) const;

        virtual bool size_equals(std::size_t size) const;

        virtual bool item_equals(std::size_t index, const unity_value &value) const;

        virtual void compare(const unity_value &value, delta_store &store) const;

        virtual void compare(const unity_array_value &value, delta_store &store) const;
    };
}
