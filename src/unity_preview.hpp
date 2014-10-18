#pragma once

#include "pod_vector.hpp"

#include <cstdint>

namespace zizany {
    class json_writer;

    class unity_preview {
    public:
        std::int32_t asset_id;
        std::int32_t magic_int_1;
        std::int32_t magic_int_2;
        std::int32_t magic_int_3;
        pod_vector<unsigned char> data;
        struct {
            std::uint32_t offset;
            std::uint32_t size;
        } file_layout;

        unity_preview();

        void print(json_writer &writer, const bool print_layout, const bool print_magic) const;
    };
}
