#pragma once

#include "../delta.hpp"
#include "../delta_set_operation.hpp"
#include "../member_path.hpp"
#include "../../unity_value.hpp"

namespace zizany {
    class asset_members_delta : public delta {
    public:
        const delta_set_operation operation;
        const int asset_id;
        const member_path path;
        const unity_value &value;

        asset_members_delta(const delta_set_operation operation_, const int asset_id_, const member_path &path_, const unity_value &value_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
