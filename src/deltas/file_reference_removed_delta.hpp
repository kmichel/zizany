#pragma once

#include "delta.hpp"
#include "../unity_file_reference.hpp"

namespace zizany {
    class file_reference_removed_delta : public delta {
    public:
        unity_file_reference reference;

        file_reference_removed_delta(const unity_file_reference &reference_);

        virtual void print_details(json_writer &writer) const;
    };
}
