#include "unity_value.hpp"

namespace zizany {
    unity_value::unity_value(const unity_type &type_, unity_value *parent_)
            : type(type_), parent(parent_) {
    }

    unity_value::~unity_value() {
    }
}
