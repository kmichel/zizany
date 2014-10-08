#pragma once

#include "owning_vector.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace zizany {
    class json_writer;

    class unity_type_member;

    class unity_type {
    public:
        std::vector<unity_type_member> members;
        std::string type_name;
        std::int32_t type_size;
        std::int32_t magic_int_1;
        std::uint32_t magic_bitset_2;
        bool is_array;

        unity_type();

        bool requires_padding() const;

        bool is_simple() const;

        void print(json_writer &writer, bool print_defauts, bool print_magic) const;
    };

    class unity_type_member {
    public:
        std::string name;
        unity_type type;
    };

}
