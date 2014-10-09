#pragma once

#include "delta.hpp"
#include "../delta_set_operation.hpp"
#include "../diff/type_identity.hpp"
#include "../values/unity_value.hpp"

namespace zizany {
    class assets_delta : public delta {
    public:
        delta_set_operation operation;
        int asset_id;
        type_identity type;
        const unity_value &value;

        assets_delta(const delta_set_operation operation_, const int asset_id_, const type_identity &type_, const unity_value &value_);

        virtual void print_details(json_writer &writer) const;
    };
}
