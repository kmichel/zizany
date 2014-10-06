#pragma once

#include "delta.hpp"

namespace zizany {
    class file_magic_changed_delta : public delta {
    public:
        int magic_id;
        int old_value;
        int new_value;

        file_magic_changed_delta(const int magic_id_, const int old_value_, const int new_value_);

        virtual void print_details(json_writer &writer) const;
    };
}
