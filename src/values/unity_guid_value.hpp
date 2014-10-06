#pragma once

#include "unity_value.hpp"
#include "../guid.hpp"

namespace zizany {

    class unity_guid_value : public unity_value {
    public:
        guid value;

        unity_guid_value();

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value) const;

        virtual bool equals(const guid &value) const;

    };
}
