#pragma once

#include "owning_vector.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace zizany {
    class json_writer;

    class unity_type {
    public:
        owning_vector<unity_type> members;
        std::string type_name;
        std::string member_name;
        std::int32_t type_size;
        std::int32_t magic_int_1;
        std::uint32_t magic_bitset_2;
        bool is_array;

        unity_type();

        unity_type(const unity_type &) = delete;

        unity_type &operator=(const unity_type &) = delete;

        bool requires_padding() const;

        bool is_simple() const;

        void print(json_writer &writer, bool print_defauts, bool print_magic) const;
    };
}
