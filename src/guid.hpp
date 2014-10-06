#pragma once

#include <cstdint>

namespace zizany {
    class json_writer;

    struct guid {
        std::uint32_t a;
        std::uint32_t b;
        std::uint32_t c;
        std::uint32_t d;

        void print(json_writer &writer) const;
    };

    bool operator==(const guid& lhs, const guid& rhs);
}
