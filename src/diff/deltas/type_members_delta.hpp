#pragma once

#include "../delta.hpp"
#include "../delta_set_operation.hpp"
#include "../type_identity.hpp"
#include "../member_path.hpp"

namespace zizany {

    class type_members_delta : public delta {
    public:
        const delta_set_operation operation;
        const type_identity identity;
        const member_path path;
        const unity_type &added_type;

        type_members_delta(const delta_set_operation operation_, const type_identity &identity_, const member_path &path_, const unity_type &added_type_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
