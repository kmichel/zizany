#pragma once

#include "delta.hpp"
#include "../diff/member_path.hpp"

namespace zizany {
    class json_writer;

    class unity_value;

    class asset_delta : public delta {

    public:
        int asset_id;
        member_path path;
        const unity_value &old_value;
        const unity_value &new_value;

        asset_delta(const int asset_id_, const member_path &path_, const unity_value &old_value_, const unity_value &new_value_);

        virtual void print_details(json_writer &writer) const;
    };
}
