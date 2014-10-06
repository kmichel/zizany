#include "guid.hpp"

#include "json_writer.hpp"

#include <algorithm>
#include <cstdio>

namespace zizany {
    void guid::print(json_writer &writer) const {
        char buffer[33];
        std::snprintf(buffer, sizeof(buffer), "%08x%08x%08x%08x", d, c, b, a);
        std::reverse(buffer, buffer + 32);
        writer.add_string(buffer, 32);
    }

    bool operator==(const guid &lhs, const guid &rhs) {
        return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c && lhs.d == rhs.d;
    }
}
