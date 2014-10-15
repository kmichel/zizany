#pragma once

#include "delta.hpp"
#include "../diff/type_identity.hpp"

namespace zizany {
    class asset_type_delta : public delta {
    public:
        int asset_id;
        type_identity old_type;
        type_identity new_type;

        asset_type_delta(const int asset_id_, const type_identity &old_type_, const type_identity &new_type_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
