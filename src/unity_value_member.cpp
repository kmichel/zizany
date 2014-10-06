#include "unity_value_member.hpp"

#include "values/unity_value.hpp"

namespace zizany {
    unity_value_member::unity_value_member(const std::string &name_, std::unique_ptr<unity_value> &&value_)
            : name(name_),
              value(std::move(value_)) {
    }
}
