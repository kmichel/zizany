#pragma once

#include "endianness.hpp"
#include "registry.hpp"

#include <string>

namespace zizany {
    class unity_type;

    class unity_asset;

    class unity_file_reference;

    class unity_preview;

    class unity_file {
    public:
        registry<unity_type> types;
        registry<unity_asset> assets;
        registry<unity_file_reference> file_references;
        registry<unity_preview> previews;

        std::int32_t magic_int_1;
        std::int32_t magic_int_2;
        std::int32_t magic_int_3;
        std::string unity_version;
        endianness file_endianness;
        struct {
            std::uint32_t metadata_size;
            std::uint32_t previews_start;
            std::uint32_t assets_start;
        } artifact_data;

        unity_file();

        unity_file(const unity_file &) = delete;

        unity_file &operator=(const unity_file &) = delete;
    };
}
