#pragma once

#include "../delta.hpp"
#include "../../guid.hpp"
#include "../../unity_file_reference_properties.hpp"

namespace zizany {
    class file_reference_delta : public delta {
    public:
        const guid file_guid;
        const unity_file_reference_properties old_properties;
        const unity_file_reference_properties new_properties;

        file_reference_delta(const guid &file_guid_, const unity_file_reference_properties &old_properties_, const unity_file_reference_properties &new_properties_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
