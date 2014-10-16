#pragma once

#include "../delta.hpp"

namespace zizany {
    class file_magic_delta : public delta {
    public:
        const int magic_id;
        const int old_value;
        const int new_value;

        file_magic_delta(const int magic_id_, const int old_value_, const int new_value_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };
}
