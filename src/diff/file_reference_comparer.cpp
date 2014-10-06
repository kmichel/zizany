#include "file_reference_comparer.hpp"

#include "../unity_file_reference.hpp"

namespace zizany {
    bool are_file_references_equal(const unity_file_reference &base, const unity_file_reference &other) {
        if (&base != &other) {
            if (base.path != other.path)
                return false;
            if (base.magic_byte_1 != other.magic_byte_1)
                return false;
            if (base.magic_int_2 != other.magic_int_2)
                return false;
        }
        return true;
    }
}
