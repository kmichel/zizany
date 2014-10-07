#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace zizany {
    class unity_value;

    class unity_asset {
    public:
        std::int32_t type_id;
        std::int32_t type_id_2;
        std::unique_ptr<unity_value> value;
        std::vector<char> unparsed_value;
        struct {
            std::uint32_t offset;
            std::uint32_t size;
        } file_layout;

        unity_asset();

        unity_asset(const unity_asset &) = delete;

        unity_asset &operator=(const unity_asset &) = delete;
    };
}
