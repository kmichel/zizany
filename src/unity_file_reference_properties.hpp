#pragma once

#include <cstdint>
#include <string>

namespace zizany {
    class json_writer;

    class unity_file_reference_properties {
    public:
        std::string path;
        std::int32_t magic_int_2;
        unsigned char magic_byte_1;

        void print(json_writer &writer, bool print_magic) const;
    };

    bool operator==(const unity_file_reference_properties &lhs, const unity_file_reference_properties &rhs);

    bool operator!=(const unity_file_reference_properties &lhs, const unity_file_reference_properties &rhs);
}
