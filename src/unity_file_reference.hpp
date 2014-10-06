#pragma once

#include "guid.hpp"

#include <cstdint>
#include <string>

namespace zizany {
    class unity_file_reference {
    public:
        std::string path;
        guid file_guid;
        std::int32_t magic_int_2;
        unsigned char magic_byte_1;

        unity_file_reference();
    };
}
