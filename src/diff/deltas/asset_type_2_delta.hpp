#pragma once

#include "../delta.hpp"

namespace zizany {
    class asset_type_2_delta : public delta {
    public:
        int asset_id;
        int old_type_2;
        int new_type_2;

        asset_type_2_delta(const int asset_id_, const int old_type_2_, const int new_type_2_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
