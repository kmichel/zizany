#pragma once

#include "../delta.hpp"
#include "../delta_sequence_operation.hpp"
#include "../member_path.hpp"

namespace zizany {
    class unity_value;

    class sequence_delta : public delta {
    public:
        delta_sequence_operation operation;
        int asset_id;
        member_path path;
        int position;
        const unity_value &value;

        sequence_delta(const delta_sequence_operation operation_, const int asset_id_, const member_path &path_, const int position_, const unity_value &value_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
