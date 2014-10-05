#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "owning_vector.hpp"

namespace zizany {
    class unity_type {
    public:
        unity_type *parent;
        owning_vector<unity_type> members;
        std::string type_name;
        std::string member_name;
        std::int32_t type_size;
        std::int32_t magic_int_1;
        std::uint32_t magic_bitset_2;
        bool is_array;

        unity_type(unity_type *parent_);

        unity_type(const unity_type &) = delete;

        unity_type &operator=(const unity_type &) = delete;

        bool requires_padding() const {
            return (magic_bitset_2 & 0x4000) != 0;
        }

        bool is_simple() const {
            return members.size() == 0 && type_size != 0;
        }
    };
}
