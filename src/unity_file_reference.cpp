#include "unity_file_reference.hpp"

namespace zizany {
    unity_file_reference::unity_file_reference()
            : path(),
              file_guid(),
              magic_int_2(),
              magic_byte_1() {
    }

    bool operator==(const unity_file_reference &lhs, const unity_file_reference &rhs) {
        return lhs.path == rhs.path && lhs.file_guid == rhs.file_guid && lhs.magic_int_2 == rhs.magic_int_2 && lhs.magic_byte_1 == rhs.magic_byte_1;
    }

    bool operator!=(const unity_file_reference &lhs, const unity_file_reference &rhs) {
        return lhs.path != rhs.path || lhs.file_guid != rhs.file_guid || lhs.magic_int_2 != rhs.magic_int_2 || lhs.magic_byte_1 != rhs.magic_byte_1;
    }
}
