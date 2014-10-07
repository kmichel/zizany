#pragma once

#include <cstdint>
#include <vector>

namespace zizany {
    class unity_preview {
    public:
        std::int32_t asset_id;
        std::int32_t magic_int_1;
        std::int32_t magic_int_2;
        std::int32_t magic_int_3;
        std::vector<unsigned char> data;
        struct {
            std::uint32_t offset;
            std::uint32_t size;
        } file_layout;

        unity_preview();

        unity_preview(const unity_preview &) = delete;

        unity_preview &operator=(const unity_preview &) = delete;
    };
}
