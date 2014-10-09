#pragma once

#include "delta.hpp"

namespace zizany {
    class file_layout_delta : public delta {
    public:
        file_layout_delta();

        virtual void print_action(json_writer &action) const;

        virtual void print_details(json_writer &writer) const;
    };
}
