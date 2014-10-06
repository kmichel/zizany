#pragma once

#include "unity_value.hpp"
#include "../asset_reference.hpp"

namespace zizany {
    class unity_asset_reference_value : public unity_value {
    public:
        asset_reference value;

        unity_asset_reference_value();

        virtual void print(json_writer &writer) const;

        virtual bool equals(const unity_value &value) const;

        virtual bool equals(const asset_reference &value) const;
    };
}
