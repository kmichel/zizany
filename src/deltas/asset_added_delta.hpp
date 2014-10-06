#pragma once

#include "delta.hpp"

#include "../diff/type_identity.hpp"
#include "../values/unity_value.hpp"

namespace zizany {
    class asset_added_delta : public delta {
    public:
        int asset_id;
        type_identity type;
        const unity_value &value;

        asset_added_delta(const int asset_id_, const type_identity &type_, const unity_value &value_);

        virtual void print_details(json_writer &writer) const;
    };
}
