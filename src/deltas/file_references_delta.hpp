#pragma once

#include "delta.hpp"
#include "../delta_set_operation.hpp"
#include "../unity_file_reference.hpp"

namespace zizany {
    class file_references_delta : public delta {
    public:
        delta_set_operation operation;
        unity_file_reference reference;

        file_references_delta(delta_set_operation operation_, const unity_file_reference &reference_);

        virtual void print_action(json_writer &action) const;

        virtual void print_details(json_writer &writer) const;
    };
}
