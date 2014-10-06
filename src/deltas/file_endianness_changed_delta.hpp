#pragma once

#include "delta.hpp"
#include "../endianness.hpp"

namespace zizany {
    class file_endianness_changed_delta : public delta {
    public:
        endianness old_endianness;
        endianness new_endianness;

        file_endianness_changed_delta(const endianness old_endianness_, const endianness new_endianness_);

        virtual void print_details(json_writer &writer) const;
    };

}
