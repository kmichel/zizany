#pragma once

#include <cstdint>

namespace zizany {
    struct guid {
        std::uint32_t a;
        std::uint32_t b;
        std::uint32_t c;
        std::uint32_t d;
    };

    bool operator==(const guid& lhs, const guid& rhs);
}
