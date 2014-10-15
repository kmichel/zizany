#pragma once

#include "delta.hpp"
#include "../delta_set_operation.hpp"
#include "../diff/type_identity.hpp"
#include "../unity_type.hpp"

namespace zizany {

    class types_delta : public delta {
    public:
        delta_set_operation operation;
        type_identity identity;
        unity_type type;

        types_delta(const delta_set_operation operation_, const type_identity &identity_, const unity_type &type_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
