#include "guid.hpp"

namespace zizany {
    bool operator==(const guid &lhs, const guid &rhs) {
        return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c && lhs.d == rhs.d;
    }
}
