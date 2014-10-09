#pragma once

#include "delta.hpp"
#include "../unity_file_reference.hpp"

namespace zizany {
    class file_reference_delta : public delta {
    public:
        unity_file_reference old_reference;
        unity_file_reference new_reference;

        file_reference_delta(const unity_file_reference &old_reference_, const unity_file_reference &new_reference_);

        virtual void print_details(json_writer &writer) const;
    };
}
