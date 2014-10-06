#pragma once

#include "unity_value.hpp"

namespace zizany {
    class unity_asset_reference_value : public unity_value {
    public:
        int file_reference_id;
        int asset_id;

        unity_asset_reference_value(const unity_type &type_, unity_value *parent_);

        virtual void print(json_writer &writer) const;
    };
}
