#pragma once

#include "../delta.hpp"
#include "../../endianness.hpp"

namespace zizany {
    class file_endianness_delta : public delta {
    public:
        const endianness old_endianness;
        const endianness new_endianness;

        file_endianness_delta(const endianness old_endianness_, const endianness new_endianness_);

        virtual void print_action(json_writer &writer) const;

        virtual void print_details(json_writer &writer) const;
    };

}
