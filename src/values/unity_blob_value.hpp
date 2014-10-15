#pragma once

#include "../unity_value.hpp"

#include <vector>

namespace zizany {
    class unity_blob_value : public unity_value {
    public:
        std::vector<char> data;

        unity_blob_value();

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value) const;

        virtual bool equals(const std::vector<char> &value) const;
    };
}
