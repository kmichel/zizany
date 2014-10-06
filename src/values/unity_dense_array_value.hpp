#pragma once

#include "unity_value.hpp"
#include "../json_writer.hpp"

#include <vector>

namespace zizany {
    template<typename element_type>
    class unity_dense_array_value : public unity_value {
    public:
        std::vector<element_type> elements;

        unity_dense_array_value(const unity_type &type_)
                : unity_value(type_), elements() {
        }

        virtual void print(json_writer &writer) const {
            writer.start_array(/*force_inline:*/true);
            for (element_type element : elements)
                writer.add_number(element);
            writer.end_array();
        }
    };

    template<typename value_type>
    std::unique_ptr<unity_dense_array_value<value_type>>
    make_dense_array_value(const unity_type &type) {
        return std::unique_ptr<unity_dense_array_value<value_type>>
                (new unity_dense_array_value<value_type>(type));
    }
}
