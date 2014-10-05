#pragma once

#include <cstdint>
#include <string>

namespace zizany {
    class unity_file_reference {
    public:
        std::string path;
        struct {
            std::uint32_t a;
            std::uint32_t b;
            std::uint32_t c;
            std::uint32_t d;
        } guid;
        std::int32_t magic_int_2;
        unsigned char magic_byte_1;

        unity_file_reference();

        unity_file_reference(const unity_file_reference &) = delete;

        unity_file_reference &operator=(const unity_file_reference &) = delete;
    };
}
