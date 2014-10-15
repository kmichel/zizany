#pragma once

#include "../unity_value.hpp"

#include <vector>

namespace zizany {
    class unity_string_value : public unity_value {
    public:
        std::vector<char> chars;

        unity_string_value();

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value) const;

        virtual bool equals(const std::string &value) const;
    };
}
