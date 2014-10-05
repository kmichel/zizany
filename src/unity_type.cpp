#include "unity_type.hpp"

namespace zizany {
    unity_type::unity_type(unity_type *parent_)
            : parent(parent_),
              members(),
              type_name(),
              member_name(),
              type_size(),
              magic_int_1(),
              magic_bitset_2() {
    }
}
