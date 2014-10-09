#pragma once

#include "delta.hpp"
#include "../delta_set_operation.hpp"
#include "../diff/member_path.hpp"
#include "../values/unity_value.hpp"

namespace zizany {
    class asset_members_delta : public delta {
    public:
        delta_set_operation operation;
        int asset_id;
        member_path path;
        const unity_value &value;

        asset_members_delta(const delta_set_operation operation_, const int asset_id_, const member_path &path_, const unity_value &value_);

        virtual void print_action(json_writer &action) const;

        virtual void print_details(json_writer &writer) const;
    };
}
