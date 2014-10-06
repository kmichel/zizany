#pragma once

#include "delta.hpp"

#include "../diff/member_path.hpp"
#include "../values/unity_value.hpp"

namespace zizany {
    class asset_member_added_delta : public delta {
    public:
        int asset_id;
        member_path path;
        const unity_value &value;

        asset_member_added_delta(const int asset_id_, const member_path &path_, const unity_value &value_);

        virtual void print_details(json_writer &writer) const;
    };
}
