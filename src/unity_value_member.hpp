#pragma once

#include <memory>
#include <string>

namespace zizany {
    class unity_value;

    class unity_value_member {
    public:
        const std::string name;
        std::unique_ptr<unity_value> value;

        unity_value_member(const std::string &name_, std::unique_ptr<unity_value> &&value_);
    };
}
