#pragma once

#include "unity_value.hpp"
#include "../json_writer.hpp"

namespace zizany {
    template<typename value_type>
    class unity_simple_value : public unity_value {
    public:
        value_type value;

        unity_simple_value(value_type value_)
                : unity_value(), value(value_) {
        }

        virtual void print(json_writer &writer) const {
            writer.add_number(value);
        }
    };

    template<typename value_type>
    std::unique_ptr<unity_simple_value<value_type>>
    make_simple_value(value_type value) {
        return std::unique_ptr<unity_simple_value<value_type>>
                (new unity_simple_value<value_type>(value));
    }


}
